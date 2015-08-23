#include "REPL.h"
#include "ConsoleWriter.h"
#include <iostream>
#include <parser/Parser.h>
#include <common/Errors.h>
#include <semantics/SemanticAnalyzer.h>
#include <semantics/ScopedNodes.h>
#include <cassert>
#include <ast/ast.h>
#include <semantics/GlobalScope.h>
#include <semantics/FunctionOverloadedSymbol.h>
#include <semantics/FunctionSymbol.h>

using namespace std;
using namespace Swallow;


REPL::REPL(const ConsoleWriterPtr& out)
    :out(out), canQuit(false)
{
    compiler = SwallowCompiler::newCompiler(L"repl");
    initCommands();
    resultId = 0;

}
REPL::~REPL()
{
    delete compiler;
}

void REPL::repl()
{
    wstring line;
    int id = 1;
    out->printf(L"Welcome to Swallow! Type :help for assistance.\n");
    while(!canQuit && !wcin.eof())
    {
        out->printf(L"%3d> ", id);
        getline(wcin, line);
        if(line.empty())
            continue;
        if(line[0] == ':')
        {
            evalCommand(line.substr(1));
            continue;
        }
        eval(line);
        dumpCompilerResults(line);
        id++;
    }
}

void REPL::eval(const wstring& line)
{
    compiler->getCompilerResults()->clear();
    compiler->clearSources();
    compiler->addSource(L"<eval>", line);
    vector<ProgramPtr> programs;
    if(!compiler->compile(programs))
        return;
    dumpProgram(programs[0]);
}
void REPL::dumpProgram(const ProgramPtr& program)
{
    SymbolScope* scope = static_pointer_cast<ScopedProgram>(program)->getScope();
    assert(scope != nullptr);
    out->setForegroundColor(ConsoleColor::Cyan);
    for(const StatementPtr& st : *program)
    {
        SymbolPtr sym = nullptr;
        switch(st->getNodeType())
        {
            case NodeType::Identifier:
            {
                IdentifierPtr id = static_pointer_cast<Identifier>(st);
                sym = scope->lookup(id->getIdentifier());
                dumpSymbol(sym);
                break;
            }
            case NodeType::ValueBindings:
            {
                ValueBindingsPtr vars = static_pointer_cast<ValueBindings>(st);
                for(const ValueBindingPtr& var : *vars)
                {
                    if(IdentifierPtr id = dynamic_pointer_cast<Identifier>(var->getName()))
                    {
                        sym = scope->lookup(id->getIdentifier());
                        dumpSymbol(sym);
                    }
                }
                break;
            }
            default:
            {
                if(PatternPtr pat = dynamic_pointer_cast<Pattern>(st))
                {
                    if(pat->getType() && !Type::equals(compiler->getSymbolRegistry()->getGlobalScope()->Void(), pat->getType()))
                    {
                        wstringstream s;
                        s<<L"$R"<<(resultId++);
                        SymbolPlaceHolderPtr sym(new SymbolPlaceHolder(s.str(), pat->getType(), SymbolPlaceHolder::R_LOCAL_VARIABLE, 0));
                        dumpSymbol(sym);
                    }
                }
                break;
            }
        }
    }
    out->reset();
}

void REPL::dumpSymbol(const SymbolPtr& sym)
{
    if(sym && sym->getType())
    {
        wstring type = sym->getType()->toString();
        out->printf(L"%ls : %ls\n", sym->getName().c_str(), type.c_str());
    }
}


void REPL::dumpCompilerResults(const std::wstring& code)
{
    dumpCompilerResults(compiler->getCompilerResults(), out);
}
void REPL::dumpCompilerResults(CompilerResults* compilerResults, const ConsoleWriterPtr& out)
{
    for(auto res : *compilerResults)
    {
        //separate source code by lines
        vector<wstring> lines;
        wstringstream stream(res.sourceFile->code);
        wstring line;
        while(getline(stream, line))
        {
            lines.push_back(line);
        }

        out->setForegroundColor(ConsoleColor::White);
        out->printf(L"%d:%d: ", res.line, res.column);
        switch(res.level)
        {
            case ErrorLevel::Fatal:
                out->setForegroundColor(ConsoleColor::Red);
                out->printf(L"fatal");
                break;
            case ErrorLevel::Error:
                out->setForegroundColor(ConsoleColor::Red, ConsoleIntensity::Bright);
                out->printf(L"error");
                break;
            case ErrorLevel::Note:
                out->setForegroundColor(ConsoleColor::White);
                out->printf(L"note");
                break;
            case ErrorLevel::Warning:
                out->setForegroundColor(ConsoleColor::Yellow);
                out->printf(L"warning");
                break;
        }
        out->setForegroundColor(ConsoleColor::White, ConsoleIntensity::Bright);
        out->printf(L": ");
        wstring msg = Errors::format(res.code, res.items);
        out->printf(L"%ls\n", msg.c_str());
        out->reset();
        if(res.line > 0 && res.line <= (int)lines.size())
        {
            wstring line = lines[res.line - 1];
            out->printf(L"%ls\n", line.c_str());
            for(int i = 0; i < res.column - 1; i++)
            {
                out->printf(L" ");
            }
            out->printf(L"^\n");
        }
        for(int i = 1; i < res.column; i++)
        {
            out->printf(L" ");
        }
        out->setForegroundColor(ConsoleColor::Green);
        out->printf(L"^\n");
        out->reset();
    }
}


void REPL::evalCommand(const wstring &command)
{
    wstring cmd = command;
    wstring args;
    wstring::size_type pos = command.find(L' ');
    if(pos != wstring::npos)
    {
        cmd = command.substr(0, pos);
        args = command.substr(pos + 1, command.size() - pos - 1);
    }
    auto iter = methods.find(cmd);
    if(iter == methods.end())
    {
        out->printf(L"error: '%ls' is not a valid command.\n", cmd.c_str());
        return;
    }
    (this->*iter->second)(args);
}

void REPL::initCommands()
{
    methods.insert(make_pair(L"help", &REPL::commandHelp));
    methods.insert(make_pair(L"quit", &REPL::commandQuit));
    methods.insert(make_pair(L"exit", &REPL::commandQuit));
    methods.insert(make_pair(L"symbols", &REPL::commandSymbols));
}
void REPL::commandHelp(const wstring& args)
{
    out->printf(L"The Swallow REPL (Read-Eval-Print-Loop) acts like an interpreter.  Valid statements, expressions, and declarations.\n");
    out->printf(L"Compiler and execution is not finished yet.");
    out->printf(L"\n");
    out->printf(L"The complete set of commands are also available as described below.  Commands must be prefixed with a colon at the REPL prompt (:quit for example.) \n\n\n");
    out->printf(L"REPL commands:\n");
    out->printf(L"  help              -- Show a list of all swallow commands, or give details about specific commands.\n");
    out->printf(L"  symbols           -- Dump symbols\n");
    out->printf(L"  quit              -- Quit out of the Swallow REPL.\n\n");
}
void REPL::commandQuit(const wstring& args)
{
    canQuit = true;
}

static void dumpSymbol(const wstring& name, const SymbolPtr& sym, const ConsoleWriterPtr& out)
{
    const wchar_t* kind = L"?????";

    if(dynamic_pointer_cast<Type>(sym))
        kind = L"Type";
    else if(dynamic_pointer_cast<SymbolPlaceHolder>(sym))
        kind = L"Placeholder";
    else if(dynamic_pointer_cast<FunctionSymbol>(sym))
        kind = L"Function";

    out->setForegroundColor(ConsoleColor::White , ConsoleIntensity::Bright);
    out->printf(L"%10ls\t", name.c_str());
    out->setForegroundColor(ConsoleColor::Magenta , ConsoleIntensity::Bright);
    out->printf(L"%7ls\t", kind);

    if(sym->getType())
    {
        wstring type = sym->getType()->toString();
        out->setForegroundColor(ConsoleColor::Yellow, ConsoleIntensity::Bright);
        out->printf(L"%ls\t", type.c_str());
    }


    out->setForegroundColor(ConsoleColor::Cyan, ConsoleIntensity::Bright);
    static const SymbolFlags flags[] = {SymbolFlagInitializing, SymbolFlagInitialized, SymbolFlagMember, SymbolFlagWritable, SymbolFlagReadable, SymbolFlagTemporary,
            SymbolFlagHasInitializer, SymbolFlagStatic, SymbolFlagInit};
    static const wchar_t* flagNames[] = {L"initializing", L"initialized", L"member", L"writable", L"readable", L"temporary", L"has_initializer", L"static", L"init"};
    for(int i = 0; i < sizeof(flags) / sizeof(flags[0]); i++)
    {
        if(sym->hasFlags(flags[i]))
            out->printf(L"%ls,", flagNames[i]);
    }

    out->printf(L"\n");
    out->reset();
}
static void dumpSymbols(SymbolScope* scope, const ConsoleWriterPtr& out)
{
    for(auto entry : scope->getSymbols())
    {
        if(FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(entry.second))
        {
            for(const FunctionSymbolPtr& func : *funcs)
            {
                dumpSymbol(entry.first, func, out);
            }
        }
        else
        {
            dumpSymbol(entry.first, entry.second, out);
        }
    }
}
void REPL::commandSymbols(const wstring& args)
{
    SymbolScope* scope;
    if(args == L"global")
        scope = compiler->getSymbolRegistry()->getGlobalScope();
    else
        scope = compiler->getScope();
    dumpSymbols(scope, out);
}
