#ifndef _SLICE_TYPE_INCLUDED_

#define _SLICE_TYPE_INCLUDED_
typedef
    enum
    {
        Program,
        Variable,
        Procedure,
        Function,
        Begin,
        End,
        Point,
        Semicolon,
        Colon,
        Identifier,
        Number,
        Unknown
    } SliceType;

#endif // _SLICE_TYPE_INCLUDED_
