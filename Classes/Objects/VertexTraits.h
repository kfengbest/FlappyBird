/*
    Copyright 2012 NAGA.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by NAGA or its suppliers or 
	licensors.  Title to the Material remains with NAGA or its suppliers and 
	licensors.  The Material is protected by worldwide copyright laws and 
	treaty provisions.  No part of the Material may be used, copied, reproduced, 
	modified, published, uploaded, posted, transmitted, distributed, or 
	disclosed in any way without NAGA's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by NAGA in
    writing.
*/

/*
	Author		:	Yu Li
	Description	:	Vertex Traits for cocos2d Renderer
	History		:	2014, Initial implementation.
*/
#ifndef __KOGO_VertexTraits_H__
#define __KOGO_VertexTraits_H__

#include "cocos2d.h"
#include "VertexTypes.h"

template <typename _Vertex>
    class VertexTraits
    {
    public:
        static INLINE int       FormatBits()        { return 0; }
        static INLINE GLenum    Type(int)           { return 0; }
        static INLINE int       Size(int)           { return 0; }
        static INLINE int       Offset(int )        { return 0; }
        static INLINE bool      Normalized(int)     { return false; }
    };

    /// the most simple vertex type which contains only position attribute
    /// might be useful for lines 
template <>
    class VertexTraits<cocos2d::Vertex2F>
    {
    public:
        typedef cocos2d::Vertex2F vertex_type;

    public:
        static INLINE int       FormatBits()        { return GL::VERTEX_ATTRIB_FLAG_POSITION; }
        static INLINE GLenum    Type(int bit)       { return GL_FLOAT; }
        static INLINE int       Size(int bit)       { return 2; }        
        static INLINE bool      Normalized(int bit) { return GL_FALSE; }
        static INLINE int       Offset(int bit)     { return 0; }
    };
    
    /// vertex traits for cocos2d::V2F_C4B_T2F
template <>
    class VertexTraits<cocos2d::V2F_C4B_T2F>
    {
    public:
        typedef cocos2d::V2F_C4B_T2F vertex_type;

    public:
        static INLINE int       FormatBits()        { return GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX; }
        static INLINE GLenum    Type(int bit)       { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? GL_UNSIGNED_BYTE : GL_FLOAT; }
        static INLINE int       Size(int bit)       { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? 4 : 2; }        
        static INLINE bool      Normalized(int bit) { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? GL_TRUE : GL_FALSE; }
        static INLINE int       Offset(int bit)
        { 
            static int Offsets[] = { offsetof(vertex_type,vertices), offsetof(vertex_type,colors), offsetof(vertex_type, texCoords) };
            return Offsets[bit]; 
        }        
    };    

    /// vertex traits for cocos2d::V2F_C4F_T2F
template <>
    class VertexTraits<cocos2d::V2F_C4F_T2F>
    {
    public:
        typedef cocos2d::V2F_C4B_T2F vertex_type;

    public:
        static INLINE int       FormatBits()        { return GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX; }
        static INLINE GLenum    Type(int bit)       { return GL_FLOAT; }
        static INLINE int       Size(int bit)       { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? 4 : 2; }        
        static INLINE bool      Normalized(int b)   { return GL_FALSE; }
        static INLINE int       Offset(int bit)
        { 
            static int Offsets[] = { offsetof(vertex_type,vertices), offsetof(vertex_type,colors), offsetof(vertex_type, texCoords) };
            return Offsets[bit]; 
        }        
    };        

template <>
    class VertexTraits<V2F_C4F>
    {
    public:
        typedef V2F_C4F vertex_type;

    public:
        static INLINE int       FormatBits()        { return GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR; }
        static INLINE GLenum    Type(int bit)       { return GL_FLOAT; }
        static INLINE int       Size(int bit)       { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? 4 : 2; }        
        static INLINE bool      Normalized(int b)   { return GL_FALSE; }
        static INLINE int       Offset(int bit)     { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? offsetof(vertex_type,colors) : offsetof(vertex_type,vertices); }        
    };        

template <>
    class VertexTraits<V2F_C4B>
    {
    public:
        typedef V2F_C4B vertex_type;

    public:
        static INLINE int       FormatBits()        { return GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR; }
        static INLINE GLenum    Type(int bit)       { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? GL_UNSIGNED_BYTE : GL_FLOAT; }
        static INLINE int       Size(int bit)       { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? 4 : 2; }        
        static INLINE bool      Normalized(int bit) { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? GL_TRUE : GL_FALSE; }
        static INLINE int       Offset(int bit)     { return bit == GLProgram::VERTEX_ATTRIB_COLOR ? offsetof(vertex_type,colors) : offsetof(vertex_type,vertices); }        
    };        

template <>
    class VertexTraits<V2F_T2F>
    {
    public:
        typedef V2F_T2F vertex_type;

    public:
        static INLINE int       FormatBits()        { return GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS; }
        static INLINE GLenum    Type(int bit)       { return GL_FLOAT; }
        static INLINE int       Size(int bit)       { return 2; }        
        static INLINE bool      Normalized(int bit) { return GL_FALSE; }
        static INLINE int       Offset(int bit)     { return bit == GLProgram::VERTEX_ATTRIB_POSITION ? offsetof(vertex_type,vertices) : offsetof(vertex_type,texCoords); }
    };

#endif // __KOGO_VertexTraits_H__
