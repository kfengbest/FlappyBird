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
	Description	:	Hardware Buffer encapsulation
	History		:	2014, Initial implementation.
*/
#ifndef __NAGA_VertexBuffer_H__
#define __NAGA_VertexBuffer_H__

#include "cocos2d.h"
#include "VertexTypes.h"
#include "VertexTraits.h"

template <typename _Vertex>
    class VertexFormat : public NAGA Object
    {
    public:
        typedef _Vertex vertex_type;
        typedef VertexTraits<vertex_type> VertexTraits;

    public:
        INLINE void Activate() 
        {
			int bits = formatBits();	
			if (bits & GL::VERTEX_ATTRIB_FLAG_POSITION) {
                VertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION);				
			}

			if (bits & GL::VERTEX_ATTRIB_FLAG_COLOR) {
                VertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR);
			}

			if (bits & GL::VERTEX_ATTRIB_FLAG_TEX_COORDS) {				
                VertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS);
			}
        }

		INLINE void Deactivate()
		{
			int bits = formatBits();
            if (bits & GL::VERTEX_ATTRIB_FLAG_POSITION) {
                glDisableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
            }

            if (bits & GL::VERTEX_ATTRIB_FLAG_COLOR) {
                glDisableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
            }

            if (bits & GL::VERTEX_ATTRIB_FLAG_TEX_COORDS) {				
                glDisableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORDS);
            }
		}

	private:
		INLINE int formatBits() { return VertexTraits::FormatBits(); }

        INLINE void VertexAttribPointer(int index)
        {
            glEnableVertexAttribArray(index);
            CHECK_GL_ERROR_DEBUG();
            glVertexAttribPointer(index, VertexTraits::Size(index), VertexTraits::Type(index), 
                    VertexTraits::Normalized(index), sizeof(vertex_type), (void*)VertexTraits::Offset(index));
            CHECK_GL_ERROR_DEBUG();
        }
    };

    /// <description>
    /// Vertex Buffer without VAO support 
    /// <description>
template <typename _Vertex>
    class VertexBuffer : public NAGA Object
    {
    public:
        typedef _Vertex vertex_type;
        typedef VertexFormat<vertex_type> vertex_format;

        /// utility class for safety map/Unmap
        struct Mapper
        {
        public:
            Mapper(VertexBuffer& buffer, GLenum access) 
                : mVertexBuffer(mVertexBuffer)
                , mBuffer(nullptr)
            { 
                mBuffer = buffer.Map(access); 
            }
            Mapper(Mapper&& m) 
                : mVertexBuffer(m.mVertexBuffer)
                , mBuffer(m.mBuffer)
            {
            }
            ~Mapper() { mVertexBuffer.Unmap(); }
            INLINE vertex_type* operator() () { return mBuffer; }

        private:
            VertexBuffer& mVertexBuffer;
            vertex_type* mBuffer;
        };

    public:
        VertexBuffer()
            : mVertexBufferID(0), mVertexCount(0)
        {
            glGenBuffers(1, &mVertexBufferID);
            CHECK_GL_ERROR_DEBUG();            
        }

        VertexBuffer(size_t count, vertex_type* data = nullptr, GLenum usage = GL_STATIC_DRAW)
            : mVertexBufferID(0), mVertexCount(count)
        {
            glGenBuffers(1, &mVertexBufferID);
            CHECK_GL_ERROR_DEBUG();            
            Init(data, count);
        }
        
        ~VertexBuffer() 
        {
            if (mVertexBufferID) 
            {
                glDeleteBuffers(1, &mVertexBufferID);
                CHECK_GL_ERROR_DEBUG();
                mVertexBufferID = 0;                
            }
        }

    public:
        /// <description>
        /// allocate the buffer needed  
        /// <description>
        bool Init(vertex_type* data, size_t count, GLenum usage = GL_STATIC_DRAW) 
        {
            if (!mVertexBufferID)
                return false;

            glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_type) *count, data, usage);
            CHECK_GL_ERROR_DEBUG();
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            return true;
        }

        /// <description>
        /// update the vertex buffer
        /// <description>
        void Update(GLenum access /*= GL_WRITE_ONLY*/, std::function<void(vertex_type*)> func)
        {
            Mapper m(*this, access);            
            vertex_type* buffer = m();
            if (buffer)
            {
                func(buffer);
            }
        }

        /// <description>
        /// get the vertex count 
        /// <description>
        INLINE size_t VertexCount() const {
            return mVertexCount;
        }

        /// <description>
        /// get the vertex buffer size in Bytes
        /// <description>
        INLINE size_t Size() const {
            return mVertexCount * sizeof(vertex_type);
        }

        /// <description>
        /// activate the Vertex Buffer  
        /// <description>
        INLINE void Activate() 
        {
            glBindVertexArray(0);
            CHECK_GL_ERROR_DEBUG();
            glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
            CHECK_GL_ERROR_DEBUG();
            mVertexFormat->Activate();
        }

        /// <description>
        /// activate the Vertex Buffer  
        /// <description>
        INLINE void Deactivate() 
        {
            mVertexFormat->Deactivate();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            CHECK_GL_ERROR_DEBUG();
            glBindVertexArray(0);
            CHECK_GL_ERROR_DEBUG();
        }

        /// <description>
        /// draw call with only vertex buffer 
        /// <description>
        void DrawArrays(GLenum mode = GL_TRIANGLES, int start = 0, int count = -1)
        {
            Activate();
			if (count == -1) {
				glDrawArrays(mode, start, (GLsizei)VertexCount());
            }
			else {
				glDrawArrays(mode, start, (GLsizei)count);
            }
            CHECK_GL_ERROR_DEBUG();
            Deactivate();            
        }

    private:
        /// <description>
        /// map a buffer object's data store       
        /// <description>
        /// <param name="access">
        ///     Specifies the access policy, indicating whether it will be possible to read from, write to, 
        ///     or both read from and write to the buffer object's mapped data store. 
        ///     The symbolic constant must be GL_READ_ONLY, GL_WRITE_ONLY, or GL_READ_WRITE.
        /// </param>
        vertex_type* Map(GLenum access = GL_WRITE_ONLY) 
        {
            glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
            void* buffer = glMapBuffer(GL_ARRAY_BUFFER, access);
            CHECK_GL_ERROR_DEBUG();
            return (vertex_type*)buffer;
        }

        /// <description>
        /// A mapped data store must be unmapped with glUnmapBuffer before its buffer object is used. 
        /// Otherwise an error will be generated by any GL command that attempts to dereference the buffer 
        /// object's data store. When a data store is unmapped, the pointer to its data store becomes invalid. 
        /// glUnmapBuffer returns GL_TRUE unless the data store contents have become corrupt during the time 
        /// the data store was mapped. This can occur for system-specific reasons that affect the availability 
        /// of graphics memory, such as screen mode changes. In such situations, GL_FALSE is returned and the 
        /// data store contents are undefined. An application must detect this rare condition and reinitialize 
        /// the data store.
        /// <description>
        bool Unmap()
        {
            bool ret = glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE;
            CHECK_GL_ERROR_DEBUG();
            return ret;
        }
        
    private:        
        /// vertex buffer identifier 
        unsigned int mVertexBufferID;
        /// vertex count 
        size_t mVertexCount;
        /// <description>
        /// vertex format should be retrieved from the VertexFormat cache 
        /// </description>
        NAGA SmartPointer<vertex_format> mVertexFormat;
    };    

template <typename _Ty>
    typename VertexBuffer<_Ty>::Mapper 
        Map(VertexBuffer<_Ty>& buffer, GLenum access = GL_WRITE_ONLY)
        {
            typedef VertexBuffer<_Ty> VertexBuffer;
            return VertexBuffer::Mapper(buffer, access);
        }

    /// <description>
    /// Index Buffer 
    /// <description>
template <typename _Ty>
    class IndexBuffer : public NAGA Object
    {       
    public:
        static_assert(std::is_integral<_Ty>::value, "short or int needed for index buffer");
        typedef _Ty index_type;
        
    public:
        IndexBuffer()
            : mIndexBufferID(nullptr), mIndexCount(count)
        {
            glGenBuffers(1, &mIndexBufferID);
            CHECK_GL_ERROR_DEBUG();
        }

        IndexBuffer(size_t count, index_type* data = nullptr, GLenum usage = GL_STATIC_DRAW)
            : mIndexBufferID(nullptr), mIndexCount(count)
        {
            glGenBuffers(1, &mIndexBufferID);
            CHECK_GL_ERROR_DEBUG();
            
        }
       
        ~IndexBuffer()
        {
            if (mIndexBufferID)
            {
                glDeleteBuffers(1, &mIndexBufferID);
                mIndexBufferID = 0;
            }            
        }

    public:
        /// <description>
        /// allocate the buffer needed  
        /// <description>
        bool Init(index_type* data, size_t count, GLenum usage = GL_STATIC_DRAW) 
        {
            if (!mIndexBufferID)
                return false;

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_type) *count, data, usage);
            CHECK_GL_ERROR_DEBUG();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            return true;
        }

        /// <description>
        /// update the vertex buffer
        /// <description>
        void Update(GLenum access /*= GL_WRITE_ONLY*/, std::function<void(index_type*)> func)
        {
            index_type* vertices = Map(access);
            if (vertices) {
                func(vertices);
            }
            Unmap();
        }

        /// <description>
        /// get the vertex count 
        /// <description>
        INLINE size_t IndexCount() const {
            return mIndexCount;
        }

        /// <description>
        /// get the vertex buffer size in Bytes
        /// <description>
        INLINE size_t Size() const {
            return mIndexCount * sizeof(index_type);
        }

        /// <description>
        /// activate the Vertex Buffer  
        /// <description>
        INLINE void Activate() 
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
            CHECK_GL_ERROR_DEBUG();
        }

        /// <description>
        /// activate the Vertex Buffer  
        /// <description>
        INLINE void Deactivate() 
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            CHECK_GL_ERROR_DEBUG();
        }

    private:
        /// <description>
        /// map a buffer object's data store       
        /// <description>
        /// <param name="access">
        ///     Specifies the access policy, indicating whether it will be possible to read from, write to, 
        ///     or both read from and write to the buffer object's mapped data store. 
        ///     The symbolic constant must be GL_READ_ONLY, GL_WRITE_ONLY, or GL_READ_WRITE.
        /// </param>
        index_type* Map(GLenum access = GL_WRITE_ONLY) 
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
            void* buffer = glMapBuffer(GL_ARRAY_BUFFER, access);
            CHECK_GL_ERROR_DEBUG();
            return (vertex_type*)buffer;
        }

        /// <description>
        /// A mapped data store must be unmapped with glUnmapBuffer before its buffer object is used. 
        /// Otherwise an error will be generated by any GL command that attempts to dereference the buffer 
        /// object's data store. When a data store is unmapped, the pointer to its data store becomes invalid. 
        /// glUnmapBuffer returns GL_TRUE unless the data store contents have become corrupt during the time 
        /// the data store was mapped. This can occur for system-specific reasons that affect the availability 
        /// of graphics memory, such as screen mode changes. In such situations, GL_FALSE is returned and the 
        /// data store contents are undefined. An application must detect this rare condition and reinitialize 
        /// the data store.
        /// <description>
        bool Unmap()
        {
            bool ret = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
            CHECK_GL_ERROR_DEBUG();
            return ret;
        }

    private:
        /// IndexBuffer ID
        unsigned int mIndexBufferID;  
        /// index count 
        size_t mIndexCount;
    };

#endif // __NAGA_HardwareBuffer_H__
