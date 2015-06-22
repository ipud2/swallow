/* TypeBuilder.h --
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
#ifndef TYPE_BUILDER_H
#define TYPE_BUILDER_H
#include "Type.h"


SWALLOW_NS_BEGIN

class SWALLOW_EXPORT TypeBuilder : public Type
{
public://Constructors
    TypeBuilder(Category category);
public:
    void setModuleName(const std::wstring& name);

    void setInitializer(const FunctionOverloadedSymbolPtr& initializer);

    void setParentType(const TypePtr& type);

    void initAlias(const TypeNodePtr& decl, std::shared_ptr<TypeResolver> typeResolver);
    /*!
     * Which type declared this
     */
    void setDeclaringType(const TypePtr& type);

    void setDeinit(const FunctionSymbolPtr& deinit);

    void setInnerType(const TypePtr& type);

    void setGenericArguments(const GenericArgumentPtr& arguments);
    
    void setGenericDefinition(const GenericDefinitionPtr& def);

    /*!
     * One generic type can be specialized to different concrete types with different generic arguments
     * This can be used to cache the varying final specialized types.
     */
    void addSpecializedType(const GenericArgumentPtr& arguments, const TypePtr& type);

    /*!
     * Adds a protocol that this type conform to
     */
    void addProtocol(const TypePtr& protocol);
    
    std::vector<int>& getProtocolFlags();

    /*!
     * Add function's parameter if it's a function type
     */
    void addParameter(const Parameter& param);
    void setSelfType(const TypePtr& type);

    void addMember(const std::wstring& name, const SymbolPtr& member);
    void addMember(const SymbolPtr& symbol);
    void addParentTypesFrom(const TypePtr& type);
    void addParentType(const TypePtr& type, int distance = 1);
    /*!
     * Add a subscript to this type
     */
    void addSubscript(const Subscript& subscript);

    /*!
     * add a new enum case
     */
    void addEnumCase(const std::wstring& name, const TypePtr& associatedType);
    /*!
     * Sets which declaration this type referenced to
     */
    void setReference(const TypeDeclarationPtr& );
};
typedef std::shared_ptr<TypeBuilder> TypeBuilderPtr;


SWALLOW_NS_END


#endif//TYPE_BUILDER_H
