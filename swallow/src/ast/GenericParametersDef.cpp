/* GenericParametersDef.cpp --
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
#include "ast/GenericParametersDef.h"
USE_SWALLOW_NS


GenericParametersDef::GenericParametersDef()
:Node(NodeType::GenericParameterDef)
{
}
GenericParametersDef::~GenericParametersDef()
{

}

void GenericParametersDef::addGenericType(const TypeIdentifierPtr& type)
{
    genericTypes.push_back(type);
}
int GenericParametersDef::numGenericTypes()const
{
    return genericTypes.size();
}
TypeIdentifierPtr GenericParametersDef::getGenericType(int i)
{
    return genericTypes[i];
}


void GenericParametersDef::addConstraint(const GenericConstraintDefPtr& constraint)
{
    constraints.push_back(constraint);
}
size_t GenericParametersDef::numConstraints()const
{
    return constraints.size();
}
GenericConstraintDefPtr GenericParametersDef::getConstraint(int i)
{
    return constraints[i];
}
const std::vector<GenericConstraintDefPtr> GenericParametersDef::getConstraints() const
{
    return constraints;
}
