//
// Project: cliblisp
// Author: bajdcc
//

#include "stdafx.h"
#include "types.h"

namespace clib {
    std::tuple<lexer_t, string_t, int> lexer_string_list[] = {
        std::make_tuple(l_none,         "none",         0),
        std::make_tuple(l_error,        "error",        0),
        std::make_tuple(l_char,         "char",         1),
        std::make_tuple(l_uchar,        "uchar",        2),
        std::make_tuple(l_short,        "short",        3),
        std::make_tuple(l_ushort,       "ushort",       4),
        std::make_tuple(l_int,          "int",          5),
        std::make_tuple(l_uint,         "uint",         6),
        std::make_tuple(l_long,         "long",         7),
        std::make_tuple(l_ulong,        "ulong",        8),
        std::make_tuple(l_float,        "float",        9),
        std::make_tuple(l_double,       "double",       10),
        std::make_tuple(l_operator,     "operator",     0),
        std::make_tuple(l_identifier,   "identifier",   0),
        std::make_tuple(l_string,       "string",       0),
        std::make_tuple(l_space,        "space",        0),
        std::make_tuple(l_newline,      "newline",      0),
        std::make_tuple(l_end,          "END",          0),
    };

    const string_t &lexer_typestr(lexer_t type) {
        assert(type >= l_none && type < l_end);
        return std::get<1>(lexer_string_list[type]);
    }

    int lexer_prior(lexer_t type) {
        assert(type >= l_none && type < l_end);
        return std::get<2>(lexer_string_list[type]);
    }

    std::tuple<operator_t, string_t, string_t> operator_string_list[] = {
        std::make_tuple(op__start, "@START", "@START"),
        std::make_tuple(op_lbrace, "{", "lbrace"),
        std::make_tuple(op_rbrace, "}", "rbrace"),
        std::make_tuple(op_lsquare, "[", "lsquare"),
        std::make_tuple(op_rsquare, "]", "rsquare"),
        std::make_tuple(op_lparan, "(", "lparan"),
        std::make_tuple(op_rparan, ")", "rparan"),
        std::make_tuple(op_quote, "`", "quote"),
        std::make_tuple(op_comma, ",", "comma"),
        std::make_tuple(op_colon, ":", "colon"),
        std::make_tuple(op_lambda, "\\", "lambda"),
        std::make_tuple(op__end, "??? unknown op", "unknown op"),
    };

    const string_t &lexer_opstr(operator_t type) {
        assert(type > op__start && type <= op__end);
        return std::get<1>(operator_string_list[type]);
    }

    const string_t &lexer_opnamestr(operator_t type) {
        assert(type > op__start && type <= op__end);
        return std::get<2>(operator_string_list[type]);
    }

    string_t err_string_list[] = {
        "@START",
        "#E !char!",
        "#E !operator!",
        "#E !digit!",
        "#E !string!",
        "@END",
    };

    const string_t &lexer_errstr(error_t type) {
        assert(type > e__start && type < e__end);
        return err_string_list[type];
    }

    bool strequ(const char * a, const char * b)
    {
        return std::strcmp(a, b) == 0;
    }
}
