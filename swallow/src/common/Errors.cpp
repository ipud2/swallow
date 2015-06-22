/* Errors.cpp --
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
#include "common/Errors.h"

USE_SWALLOW_NS

std::wstring Errors::format(int code, const std::vector<std::wstring>& items)
{
    std::wstring temp = getErrorTemplate(code);
    while(true)
    {
        int p = temp.find('%');
        if (p == -1)
            break;
        int idx = temp[p + 1] - '0';
        std::wstring item = items[idx];
        temp.replace(p, 2, item);
    }
    return temp;
}
std::wstring Errors::getErrorTemplate(int errorCode)
{
    switch(errorCode)
    {
        case Errors::E_UNEXPECTED_EOF: return L"Unexpected end-of-file";
        case Errors::E_EXPECT_2: return L"%0 is expected, but %1 found.";
        case Errors::E_UNEXPECTED_1: return L"Unexpected %0";
        case Errors::E_EXPECT_IDENTIFIER_1: return L"Identifier expected, but %0 found.";
        case Errors::E_EXPECT_KEYWORD_2: return L"Keyword %0 expected, but %1 found.";
        case Errors::E_EXPECT_EXPRESSION_1: return L"Expression expected but %0 found.";
        case Errors::E_EXPECT_OPERATOR_1: return L"Operator expected but %0 found.";
        case Errors::E_EXPECT_INTEGER_PRECEDENCE: return L"Operator's precedence must be an integer";
        case Errors::E_EXPECT_INIT_SELF_DYNAMICTYPE_IDENTIFIER_1: return L"init/self/dynamicType or member field is expected, but %0 found.";
        case Errors::E_UNDEFINED_INFIX_OPERATOR_1: return L"Undefined infix operator %0";
        case Errors::E_EXPECT_CAPTURE_SPECIFIER: return L"The capture specifier is not specified.";
        case Errors::E_EXPECT_CASE: return L"case/default is expected in switch/case statement";
        case Errors::E_GETTER_SETTER_CAN_ONLY_BE_DEFINED_FOR_A_SINGLE_VARIABLE: return L"Getter/setter can only be defined for a single variable";
        case Errors::E_UNTERMINATED_STRING_LITERAL: return L"Unterminated string literal";
        case Errors::E_UNEXPECTED_CHARACTER_A_IN_STRING_INTERPOLATION: return L"Unexpected '%0' character in string interpolation";
        case Errors::E_INVALID_ESCAPE_SEQUENCE_IN_LITERAL: return L"Invalid escape sequence in literal";
        case Errors::E_COMPUTED_PROPERTY_CANNOT_BE_DECLARED_UNDER_FOR_LOOP: return L"Computed property cannot be declared under for loop";


        case Errors::E_INVALID_REDECLARATION_1: return L"Invalid redeclaration of '%0'";
        case Errors::E_USE_OF_UNDECLARED_TYPE_1: return L"Use of undeclared type %0";
        case Errors::E_CANNOT_ASSIGN_TO_THE_RESULT_OF_THIS_EXPRESSION: return L"cannot assign to the result of this expression";
        case Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2: return L"Cannot assign to '%0' in '%1'";
        case Errors::E_CANNOT_ASSIGN_TO_A_IN_A_METHOD_1: return L"Cannot assign to '%0' in a method";
        case Errors::E_CANNOT_ASSIGN_TO_LET_VALUE_A_1: return L"Cannot assign to 'let' value '%0'";
        case Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1: return L"use of unresolved identifier '%0'";
        case Errors::E_USE_OF_UNINITIALIZED_VARIABLE_1: return L"use of local variable '%0' before its declaration";
        case Errors::E_USE_OF_INITIALIZING_VARIABLE: return L"variable used within its own initial value";
        case Errors::E_DEFINITION_CONFLICT: return L"definition conflicts with previous value";
        case Errors::E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE: return L"use of function local variable inside type declaration";
        case Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE_1: return L"tuple pattern cannot match values of the non-tuple type '%0'";
        case Errors::E_TUPLE_TYPES_HAVE_A_DIFFERENT_NUMBER_OF_ELEMENT_4: return L"tuple types '%0' and '%1' have a different number of elements (%2 vs. %3)";
        case Errors::E_OPERATOR_REDECLARED: return L"operator redeclared";
        case Errors::E_OPERATOR_PRECEDENCE_OUT_OF_RANGE: return L"'precedence' must be in the range of 0 to 255";
        case Errors::E_UNKNOWN_BINARY_OPERATOR_1: return L"operator '%0' is not a known binary operator";
        case Errors::E_IS_NOT_BINARY_OPERATOR_1: return L"'%0' is not a binary operator";
        case Errors::E_NO_OVERLOAD_ACCEPTS_ARGUMENTS_1: return L"could not find an overload for '%0' that accepts the supplied arguments";
        case Errors::E_INVALID_USE_OF_A_TO_CALL_A_VALUE_OF_NON_FUNCTION_TYPE_B_2: return L"invalid use of '%0' to call a value of non-function type '%1'";
        case Errors::E_UNMATCHED_PARAMETERS: return L"Unmatched number of supplied parameters";
        case Errors::E_UNMATCHED_PARAMETER_1: return L"Unmatched type of parameter %0";
        case Errors::E_NO_MATCHED_OVERLOAD_FOR_A_1: return L"No matched overload found for '%0'";
        case Errors::E_MISSING_ARGUMENT_LABEL_IN_CALL_1: return L"Missing argument label '%0:' in call";
        case Errors::E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL_1: return L"Extraneous argument label '%0:' in call";
        case Errors::E_EXTRANEOUS_ARGUMENT: return L"Extraneous argument in call";
        case Errors::E_INCORRECT_ARGUMENT_LABEL_IN_CALL_HAVE_A_EXPECTED_B_2: return L"incorrect argument label in call (have '%0', expected '%1')";
        case Errors::E_AMBIGUOUS_USE_1: return L"Ambiguous use of '%0'";
        case Errors::E_INOUT_ARGUMENTS_CANNOT_BE_VARIADIC: return L"Inout arguments cannot be variadic";
        case Errors::E_DOES_NOT_HAVE_A_MEMBER_2: return L"'%0' does not have a member named '%1'";
        case Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2: return L"Cannot convert expression's type '%0' to type '%1'";
        case Errors::E_LET_REQUIRES_INITIALIZER: return L"'let' declarations require an initializer expression";
        case Errors::E_CANNOT_DEFINE_AN_EMPTY_ARRAY_WITHOUT_CONTEXTUAL_TYPE: return L"cannot define an empty array without contextual type.";
        case Errors::E_CANNOT_DEFINE_AN_EMPTY_DICTIONARY_WITHOUT_CONTEXTUAL_TYPE: return L"cannot define an empty dictionary without contextual type.";
        case Errors::E_ARRAY_CONTAINS_DIFFERENT_TYPES: return L"Array contains different types";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_: return L"Type '%0' does not conform to '%1' protocol";
        case Errors::E_SUPERCLASS_MUST_APPEAR_FIRST_IN_INHERITANCE_CLAUSE_1: return L"Superclass '%0' must appear first in the inheritance clause";
        case Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_NON_CLASS_TYPE_1: return L"Inheritance from none-protocol, non-class type '%0'";
        case Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_TYPE_1: return L"Inheritance from none-protocol type '%0'";
        case Errors::E_DEFAULT_ARGUMENT_NOT_PERMITTED_IN_A_PROTOCOL_METHOD: return L"Default argument not permitted in a protocol method";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3: return L"Type %0 does not conform to protocol %1, unimplemented function %2";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_TYPE_3: return L"Type %0 does not conform to protocol %1, unimplemented type %2";
        case Errors::E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT_1: return L"Protocol %0 cannot define let constant";
        case Errors::E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY_1: return L"Protocol %0's variable must be a computed property";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_PROPERTY_3: return L"Type '%0' does not conform to protocol '%1', unimplemented property '%2'";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNWRITABLE_PROPERTY_3: return L"Type '%0' does not conform to protocol '%1', unwritable property '%2'";
        case Errors::E_TYPE_ANNOTATION_MISSING_IN_PATTERN: return L"Type annotation missing in pattern";
        case Errors::E_NESTED_TYPE_IS_NOT_ALLOWED_HERE: return L"Nested type is not allowed here";
        case Errors::E_GENERIC_TYPE_ARGUMENT_REQUIRED: return L"Generic type argument required";
        case Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_TOO_MANY_TYPE_PARAMETERS_3: return L"Generic type '%0' specialized with too many type parameters(got %1, but expected %2)";
        case Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_INSUFFICIENT_TYPE_PARAMETERS_3: return L"Generic type '%0' specialized with insufficient type parameters(got %1, but expected %2);";
        case Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1: return L"Cannot specialize non-generic type '%0'";
        case Errors::E_MULTIPLE_INHERITANCE_FROM_CLASS_2_: return L"Multiple inheritance from class '%0' and '%1'";
        case Errors::E_IS_NOT_A_MEMBER_OF_2: return L"%0 is not a member of %1";
        case Errors::E_SAME_TYPE_REQUIREMENTS_MAKES_GENERIC_PARAMETER_NON_GENERIC_1: return L"Same-type requirement makes generic parameter '%' non-generic";
        case Errors::E_PROTOCOL_CAN_ONLY_BE_USED_AS_GENERIC_CONSTRAINT_1: return L"Protocol '%0' can only be used as a generic constraint because it has Self or associated type requirements";
        case Errors::E_UNDEFINED_SUBSCRIPT_ACCESS_FOR_1: return L"Undefined subscript access for '%0'";
        case Errors::E_A_IS_NOT_A_MEMBER_TYPE_OF_B_2: return L"'%0' is not a member type of '%1'";
        case Errors::E_TYPE_A_NESTED_IN_GENERIC_TYPE_B_IS_NOT_ALLOWED_2: return L"type '%0' nested in generic type '%1' is not allowed";
        case Errors::E_GENERIC_TYPE_A_NESTED_IN_TYPE_B_IS_NOT_ALLOWED_2: return L"generic type '%0' nested in type '%1' is not allowed";
        case Errors::E_TUPLE_ACCESS_ONLY_WORKS_FOR_TUPLE_TYPE: return L"Tuple access only works for tuple type";
        case Errors::E_TUPLE_ACCESS_A_OUT_OF_RANGE_IN_B_2: return L"Tuple access '%0' out of range in '%1'";
        case Errors::E_VARLET_CANNOT_APPEAR_INSIDE_ANOTHER_VAR_OR_LET_PATTERN_1: return L"%0 cannot appear inside another var or let pattern";
        case Errors::E_EXPECT_TUPLE_OR_IDENTIFIER: return L"Expect tuple or identifier";
        case Errors::E_TYPE_ANNOTATION_DOES_NOT_MATCH_CONTEXTUAL_TYPE_A_1: return L"Type annotation does not match contextual type '%0'";
        case Errors::E_TUPLE_PATTERN_CANNOT_MATCH_VALUES_OF_THE_NON_TUPLE_TYPE_A_1: return L"Tuple pattern cannot match values of the non-tuple type '%0'";
        case Errors::E_A_IS_NOT_CONVERTIBLE_TO_B_2: return L"'%0' is not convertible to '%1'";
        case Errors::E_NO_CONTEXTUAL_TYPE_TO_ACCESS_MEMBER_A_1: return L"No contextual type to access member '%0'";
        case Errors::E_A_LABEL_IN_SWITCH_SHOULD_HAVE_AT_LEAST_ONE_STATEMENT_0: return L"'%0' label in a 'switch' should have at least one executable statement'";
        case Errors::E_SWITCH_MUST_BE_EXHAUSIVE_CONSIDER_ADDING_A_DEFAULT_CLAUSE: return L"Switch must be exhausive, consider adding a default clause";
        case Errors::E_PARTIAL_APPLICATION_OF_ENUM_CONSTRUCTOR_IS_NOT_ALLOWED: return L"partial application of enum constructor is not allowed";


        case Errors::E_MULTIPLE_ENUM_RAW_TYPES_A_AND_B_2: return L"Multiple enum raw types '%0' and '%1'";
        case Errors::E_RAW_TYPE_A_MUST_APPEAR_FIRST_IN_THE_ENUM_INHERITANCE_CLAUSE_1: return L"Raw type '%0' must appear first in the enum inheritance clause";
        case Errors::E_RAW_TYPE_A_IS_NOT_CONVERTIBLE_FROM_ANY_LITERAL_1: return L"Raw type '%0' is not convertible from any literal";
        case Errors::E_RAWREPRESENTABLE_INIT_CANNOT_BE_SYNTHESIZED_BECAUSE_RAW_TYPE_A_IS_NOT_EQUATABLE_1: return L"RawRepresentable 'init' cannot be synthesized because raw type '%0' is not Equatable";
        case Errors::E_ENUM_WITH_NO_CASES_CANNOT_DECLARE_A_RAW_TYPE: return L"An enum with no cases cannot declare a raw type";
        case Errors::E_RAW_VALUE_FOR_ENUM_CASE_MUST_BE_LITERAL: return L"Raw value for enum case must be literal";
        case Errors::E_ENUM_CASES_REQUIRE_EXPLICIT_RAW_VALUES_WHEN_THE_RAW_TYPE_IS_NOT_INTEGER_LITERAL_CONVERTIBLE: return L"Enum cases require explicit raw values when the raw type is not integer literal convertible";
        case Errors::E_ENUM_WITH_RAW_TYPE_CANNOT_HAVE_CASES_WITH_ARGUMENTS: return L"Enum with raw type cannot have cases with arguments";
        case Errors::E_ENUM_CASE_CANNOT_HAVE_A_RAW_VALUE_IF_THE_ENUM_DOES_NOT_HAVE_A_RAW_TYPE: return L"Enum case cannot have a raw value if the enum does not have a raw type";

        case Errors::E_EXPRESSION_DOES_NOT_CONFORM_TO_PROTOCOL_1: return L"Expression does not conform to protocol '%0'";
        case Errors::E_OPERAND_OF_POSTFIX_A_SHOULD_HAVE_OPTIONAL_TYPE_TYPE_IS_B_2: return L"Operand of postfix '%0' should have optional type; type is '%1'";
        case Errors::E_BOUND_VALUE_IN_A_CONDITIONAL_BINDING_MUST_BE_OF_OPTIONAL_TYPE: return L"Bound value in a conditional binding must be of Optional type";
        case Errors::E_EXPECTED_EXPRESSION_VAR_OR_LET_IN_A_CONDITION_1: return L"Expected expression, var, or let in '%0' condition";
        case Errors::E_VARIABLE_BINDING_IN_A_CONDITION_REQUIRES_AN_INITIALIZER: return L"Variable binding in a condition requires an initializer";
        case Errors::E_A_IS_NOT_IDENTICIAL_TO_B_2: return L"'%0' is not identical to '%1'";
        case Errors::E_RETURN_INVALID_OUTSIDE_OF_A_FUNC: return L"return invalid outside of a func";
        case Errors::E_SUBSCRIPT_ACCESS_ON_A_IS_NOT_WRITABLE_1: return L"Subscript access on '%0' is not writable";
        case Errors::E_DICTIONARY_KEY_CONTAINS_DIFFERENT_TYPES: return L"Dictionary key contains different types";
        case Errors::E_DICTIONARY_VALUE_CONTAINS_DIFFERENT_TYPES: return L"Dictionary value contains different types";
        case Errors::E_OPERATOR_IMPLEMENTATION_WITHOUT_MATCHING_OPERATOR_DECLARATION: return L"Operator implementation without matching operator declaration";
        case Errors::E_A_REQUIRES_A_FUNCTION_WITH_ONE_ARGUMENT_1: return L"'%0' requires a function with one argument";
        case Errors::E_UNARY_OPERATOR_IMPLEMENTATION_MUST_HAVE_A_PREFIX_OR_POSTFIX_MODIFIER: return L"Unary operator implementation must have a 'prefix' or 'postfix' modifier";
        case Errors::E_OPERATORS_MUST_HAVE_ONE_OR_TWO_ARGUMENTS: return L"Operators must have one or two arguments";
        case Errors::E_OPERATOR_MUST_BE_DECLARED_AS_PREFIX_POSTFIX_OR_INFIX: return L"Operator must be declared as 'prefix', 'postfix', or 'infix'";
        case Errors::E_A_MAY_ONLY_BE_DECLARED_AT_FILE_SCOPE_1: return L"'%0' may only be declared at file scope";



        case Errors::E_GENERIC_ARGUMENTS_ARE_NOT_ALLOWED_ON_AN_EXTENSION: return L"Generic arguments are not allowed on an extension";
        case Errors::E_PROTOCOL_A_CANNOT_BE_EXTENDED_1: return L"Protocol '%0' cannot be extended";
        case Errors::E_NON_NOMINAL_TYPE_A_CANNOT_BE_EXTENDED_1: return L"Non-nominal type '%0' cannot be extended";
        case Errors::E_EXTENSIONS_MAY_NOT_CONTAIN_STORED_PROPERTIES: return L"Extensions may not contain stored properties";
        case Errors::E_ENUMS_MAY_NOT_CONTAIN_STORED_PROPERTIES: return L"Enums may not contain stored properties";
        case Errors::E_INIT_CAN_ONLY_REFER_TO_THE_INITIALIZERS_OF_SELF: return L"'init' can only refer to the initializers of 'self'";
        case Errors::E_FUNCTION_PROCEDURES_EXPECTD_TYPE_A_DID_YOU_MEAN_TO_CALL_IT_WITH_1: return L"function produces expected type '%0'; did you mean to call it with '()'?";
        case Errors::E_LAZY_PROPERTIES_MUST_HAVE_AN_INITIALIZER: return L"Lazy properties must have an initializer";
        case Errors::E_LAZY_CANNOT_DESTRUCTURE_AN_INITIALIZER: return L"'lazy' cannot destructure an initializer";
        case Errors::E_LAZY_CANNOT_BE_USED_ON_A_LET: return L"'lazy' cannot be used on a let";
        case Errors::E_LAZY_IS_ONLY_VALID_FOR_MEMBERS_OF_A_STRUCT_OR_CLASS: return L"Lazy is only valid for members of a struct or class";
        case Errors::E_LAZY_MAY_NOT_BE_USED_ON_A_COMPUTED_PROPERTY: return L"'lazy' may not be used on a computed property";
        case Errors::E_CLASS_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE: return L"Class properties may only be declared on a type";
        case Errors::E_STATIC_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE: return L"Static properties may only be declared on a type";
        case Errors::E_STATIC_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_STRUCTS_AND_ENUMS: return L"Static properties are only allowed within structs and enums; use 'class' to declare a class property";
        case Errors::E_CLASS_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_CLASSES_AND_PROTOCOLS: return L"Class properties are only allowed within classes and protocols; use 'static' to declare a static property";
        case Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2: return L"'%0' may only be used on '%1' declaration";
        case Errors::E_A_IS_ONLY_VALID_ON_METHODS_1: return L"'%0' is only valid on methods";
        case Errors::E_A_ISNT_VALID_ON_METHODS_IN_CLASSES_OR_CLASS_BOUND_PROTOCOLS: return L"'%0' isn't valid on methods in classes or class-bound protocols";
        case Errors::E_STATIC_FUNCTIONS_MAY_NOT_BE_DECLARED_A_1: return L"Static functions may not be declared %0";
        case Errors::E_METHOD_MAY_NOT_BE_DECLARED_BOTH_MUTATING_AND_NONMUTATING: return L"Method may not be declared both mutating and nonmutating";
        case Errors::E_IMMUTABLE_VALUE_OF_TYPE_A_ONLY_HAS_MUTATING_MEMBERS_NAMED_B_2: return L"Immutable value of type '%0' only has mutating members named '%1'";
        case Errors::E_A_CAN_ONLY_BE_SPECIFIED_ON_CLASS_MEMBERS: return L"'%0' can only be specified on class members";
        case Errors::E_METHOD_DOES_NOT_OVERRIDE_ANY_METHOD_FROM_ITS_SUPERCLASS: return L"Method does not override any method from its superclass";
        case Errors::E_OVERRIDING_DECLARATION_REQUIRES_AN_OVERRIDE_KEYWORD: return L"Overriding declaration requires an 'override' keyword";
        case Errors::E_DECLARATIONS_IN_EXTENSIONS_CANNOT_OVERRIDE_YET: return L"Declarations in extensions cannot override yet";
        case Errors::E_PROPERTY_DOES_NOT_OVERRIDE_ANY_PROPERTY_FROM_ITS_SUPERCLASS: return L"Property does not override any property from its superclass";
        case Errors::E_PROPERTY_A_WITH_TYPE_B_CANNOT_OVERRIDE_A_PROPERTY_WITH_TYPE_C_3: return L"Property '%0' with type '%1' cannot override a property with type '%2'";
        case Errors::E_CANNOT_OVERRIDE_MUTABLE_PROPERTY_WITH_READONLY_PROPERTY_A_1: return L"Cannot override mutable property with read-only property '%0'";
        case Errors::E_SUBSCRIPT_DOES_NOT_OVERRIDE_ANY_SUBSCRIPT_FROM_ITS_SUPERCLASS: return L"Subscript does not override any subscript from its superclass";
        case Errors::E_INOUT_IS_ONLY_VALID_IN_PARAMTER_LISTS: return L"'inout' is only valid in parameter lists";
        case Errors::E_INHERITANCE_FROM_A_FINAL_CLASS_A_1: return L"Inheritance from a final class '%0'";
        case Errors::E_A_MODIFIER_CANNOT_BE_APPLIED_TO_THIS_DECLARATION_1: return L"'%0' modifier cannot be applied to this declaration";
        case Errors::E_ONLY_CLASSES_AND_CLASS_MEMBERS_MAY_BE_MARKED_WITH_FINAL: return L"Only classes and class members may be marked with 'final'";
        case Errors::E_CANNOT_OVERRIDE_WITH_A_STORED_PROPERTY_A_1: return L"Cannot override with a stored property '%0'";
        case Errors::E_VAR_OVERRIDES_A_FINAL_VAR: return L"Var overrides a 'final' var";
        case Errors::E_INSTANCE_METHOD_OVERRIDES_A_FINAL_INSTANCE_METHOD: return L"Instance method overrides a 'final' instance method";
        case Errors::E_SUBSCRIPT_OVERRIDES_A_FINAL_SUBSCRIPT: return L"Subscript overrides a 'final' subscript";
        case Errors::E_SUPER_INIT_CANNOT_BE_CALLED_OUTSIDE_OF_AN_INITIALIZER: return L"'super.init' cannot be called outside of an initializer";
        case Errors::E_SUPER_INIT_ISNT_CALLED_BEFORE_RETURNING_FROM_INITIALIZER: return L"Super.init isn't called before returning from initializer";
        case Errors::E_SELF_INIT_ISNT_CALLED_ON_ALL_PATHS_IN_DELEGATING_INITIALIZER: return L"Self.init isn't called on all paths in delegating initializer";
        case Errors::E_SELF_INIT_CALLED_MULTIPLE_TIMES_IN_INITIALIZER: return L"Self.init called multiple times in initializer";
        case Errors::E_INITIALIZER_DELEGATION_CAN_ONLY_OCCUR_WITHIN_AN_INITIALIZER: return L"Initializer delegation can only occur within an initializer";
        case Errors::E_DESIGNATED_INITIALIZER_FOR_A_CANNOT_DELEGATE_1: return L"Designated initializer for '%0' cannot delegate(with 'self.init'); did you mean this to be a convenience initializer?";
        case Errors::E_SUPER_INIT_CALLED_MULTIPLE_TIMES_IN_INITIALIZER: return L"Self.init called multiple times in initializer";
        case Errors::E_MUST_CALL_A_DESIGNATED_INITIALIZER_OF_THE_SUPER_CLASS_A_1: return L"Must call a designated initializer of the superclass '%0'";
        case Errors::E_CONVENIENCE_INITIALIZER_FOR_A_MUST_DELEGATE_WITH_SELF_INIT_1: return L"Convenience initializer for '%0' must delegate(with 'self.init') rather than chaining to a superclass initializer(with 'super.init')";
        case Errors::E_MISSING_RETURN_IN_A_FUNCTION_EXPECTED_TO_RETURN_A_1: return L"Missing return in a function expected to return '%0'";
        case Errors::E_PROPERTY_A_NOT_INITIALIZED_AT_SUPER_INIT_CALL_1: return L"Property '%0' not initialized at super.init call";
        case Errors::E_VARIABLE_A_USED_BEFORE_BEING_INITIALIZED_1: return L"Variable '%0' used before being initialized";
        case Errors::E_USE_OF_PROPERTY_A_IN_BASE_OBJECT_BEFORE_SUPER_INIT_INITIALIZES_IT: return L"Use of property '%0' in base object before super.init initializes it";
        case Errors::E_USE_OF_SELF_IN_DELEGATING_INITIALIZER_BEFORE_SELF_INIT_IS_CALLED: return L"Use of 'self' in delegating initializer before self.init is called";
        case Errors::E_SELF_USED_BEFORE_SUPER_INIT_CALL: return L"'self' used before super.init call";
        case Errors::E_PROPERTY_A_NOT_INITIALIZED: return L"Property '%0' not initialized";
        case Errors::E_NIL_IS_THE_ONLY_RETURN_VALUE_PERMITTED_IN_AN_INITIALIZER: return L"'nil' is the only return value permitted in an initializer";
        case Errors::E_ONLY_A_FAILABLE_INITIALIZER_CAN_RETURN_NIL: return L"Only a failable initializer can return 'nil'";
        case Errors::E_ALL_STORED_PROPERTIES_OF_A_CLASS_MUST_BE_INITIALIZED_BEFORE_RETURNING_NIL: return L"All stored properties of a class instance must be initialized before returning nil from an initializer";
        case Errors::E_A_NON_FAILABLE_INITIALIZER_CANNOT_CHAINING_TO_FAILABLE_INITIALIZER_A_WRITTEN_WITH_INIT_1: return L"A non-failable initializer cannot chaining to failable initializer '%0' written with 'init?'";
        case Errors::E_REQUIRED_INITIALIZER_IN_NON_CLASS_TYPE_A_1: return L"'required' initializer in non-class type '%0'";
        case Errors::E_REQUIRED_MODIFIER_MUST_BE_PRESENT_ON_ALL_OVERRIDES_OF_A_REQUIRED_INITIALIZER: return L"'required' modifier must be present on all overrides of a required initializer";
        case Errors::E_DUPLICATE_MODIFIER: return L"Duplicate modifier";
        case Errors::E_A_MUST_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4: return L"%0 must be declared %1 because its %2 uses a %3 type";
        case Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4: return L"%0 cannot be declared %1 because its %2 uses a %3 type";
        case Errors::E_NON_PROTOCOL_TYPE_A_CANNOT_BE_USED_WITHIN_PROTOCOL_COMPOSITION_1: return L"Non-protocol type '%0' cannot be used within 'protocol<...>'";
        case Errors::E_EXPECT_MODULE_MEMBER_NAME_AFTER_MODULE_NAME: return L"expected module member name after module name";
        case Errors::E_NO_TYPE_NAMED_A_IN_MODULE_B_2: return L"No type named '%0' in module '%1'";


        case Errors::W_CODE_AFTER_A_WILL_NEVER_BE_EXECUTED_1: return L"Code after 'return' will never be executed";
        case Errors::W_PARAM_CAN_BE_EXPRESSED_MORE_SUCCINCTLY_1: return L"'%0 %0' can be expressed more succinctly as '#%0'";
        case Errors::W_EXTRANEOUS_SHARTP_IN_PARAMETER_1: return L"Extraneous '#' in parameter: '%0' is already the keyword argument name";
    }
    return L"<unknown>";
}
