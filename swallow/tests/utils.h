/* utils.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>
#include "parser/Parser.h"
#include "ast/ast.h"
#include "ast/NodeFactory.h"
#include "common/CompilerResults.h"
#include "semantics/semantic-types.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/GlobalScope.h"
#include "semantics/ScopedNodes.h"
#include "codegen/NameMangling.h"
#include "SwallowCompiler.h"


#define ASSERT_NOT_NULL(condition) GTEST_TEST_BOOLEAN_((condition) != NULL, #condition, false, true, GTEST_FATAL_FAILURE_)
#define ASSERT_NULL(condition) GTEST_TEST_BOOLEAN_((condition) == NULL, #condition, false, true, GTEST_FATAL_FAILURE_)

void dumpCompilerResults(Swallow::CompilerResults& compilerResults);
Swallow::NodePtr parseStatement(Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str);
Swallow::ProgramPtr parseStatements(Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str);
void initTestMethods(const std::shared_ptr<Swallow::SwallowCompiler>& compiler);

Swallow::ScopedProgramPtr analyzeStatement(Swallow::SymbolRegistry& registry, Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str);
std::wstring readFile(const char* fileName);
void testInit(int argc, char** argv);
const Swallow::CompilerResult* getCompilerResultByError(Swallow::CompilerResults& results, int error);



struct Tracer
{
    Tracer(const char* file, int line, const char* func);
    ~Tracer();
    char file[1024];
    int line;
    char func[100];
    
};
#define PARSE_STATEMENT(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swallow::CompilerResults compilerResults; \
    NodePtr root = parseStatement(compilerResults, __FUNCTION__, (s));




#define SEMANTIC_ANALYZE(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    std::shared_ptr<SwallowCompiler> compiler(SwallowCompiler::newCompiler(L"test")); \
    initTestMethods(compiler); \
    compiler->addSource(L"code", s); \
    Swallow::SymbolRegistry& symbolRegistry = *compiler->getSymbolRegistry(); \
    Swallow::GlobalScope* global = symbolRegistry.getGlobalScope(); (void)global; \
    Swallow::CompilerResults& compilerResults = *compiler->getCompilerResults(); \
    NameMangling mangling(&symbolRegistry); \
    std::vector<std::shared_ptr<Swallow::Program>> roots; \
    compiler->compile(roots); \
    std::shared_ptr<Swallow::Program> root = roots[0]; \
    Swallow::SymbolScope* scope = compiler->getScope(); \
    (void)scope; \
    (void)compilerResults; \
    (void)mangling; \
    const CompilerResult* error = nullptr; \
    (void)error;
/*
#define SEMANTIC_ANALYZE(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swallow::SymbolRegistry symbolRegistry; \
    Swallow::GlobalScope* global = symbolRegistry.getGlobalScope(); (void)global; \
    Swallow::CompilerResults compilerResults; \
    NameMangling mangling(&symbolRegistry); \
    std::shared_ptr<Swallow::ScopedProgram> root = analyzeStatement(symbolRegistry, compilerResults, __FUNCTION__, s); \
    Swallow::SymbolScope* scope = root ? root->getScope() : (Swallow::SymbolScope*)nullptr; \
    (void)scope; \
    (void)mangling; \
    const CompilerResult* error = nullptr; \
    (void)error;
*/


#define SEMANTIC_ANALYZE_F(fileName) SEMANTIC_ANALYZE(readFile(fileName).c_str());
#define ASSERT_NO_ERRORS() if(0 != compilerResults.numResults()) { \
        dumpCompilerResults(compilerResults); \
        GTEST_FATAL_FAILURE_("Unexpected compilation errors."); \
    }

#define ASSERT_ERROR(e) error = getCompilerResultByError(compilerResults, e); \
    if(!error) { \
        dumpCompilerResults(compilerResults); \
        if(compilerResults.numResults()) \
            GTEST_FATAL_FAILURE_("Expect compiler result " #e); \
        else \
            GTEST_FATAL_FAILURE_("No error detected, expect compiler result " #e); \
        }



#endif//TEST_UTILS_H



