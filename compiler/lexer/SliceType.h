#ifndef _SLICE_TYPE_INCLUDED_

#define _SLICE_TYPE_INCLUDED_
#define SYMBOLS_LENGHT 13
#define RESERVED_LENGHT 33
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
        Or,
        And,
        Equal,
        Greater,
        Minor,
        Not,
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
