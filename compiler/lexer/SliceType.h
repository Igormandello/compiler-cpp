#ifndef _SLICE_TYPE_INCLUDED_

#define _SLICE_TYPE_INCLUDED_
#define SYMBOLS_LENGHT 14
#define RESERVED_LENGHT 31
typedef
    enum
    {
        Program,
        Variable,
        Procedure,
        Function,
        If,
        Then,
        While,
        Do,
        Begin,
        End,
        Point,
        Semicolon,
        Colon,
        Comma,
        LeftParenthesis,
        RightParenthesis,
        Equal,
        Greater,
        Minor,
        Exclamation,
        Plus,
        Minus,
        Times,
        Slash,
        Write,
        Read,
        True,
        False,
        Integer,
        Boolean,
        Null,
        Identifier,
        Number,
        Unknown
    }
    SliceType;

#endif // _SLICE_TYPE_INCLUDED_
