#ifndef INCLUDED_ASSBIN_CHUNKS_H
#define INCLUDED_ASSBIN_CHUNKS_H

#define ASSBIN_VERSION_MAJOR 1
#define ASSBIN_VERSION_MINOR 0

/**
@page assfile .ASS File formats

@section over Overview
Assimp provides its own interchange format, which is intended to applications which need
to serialize 3D-models and to reload them quickly. Assimp's file formats are designed to
be read by Assimp itself. They encode additional information needed by Assimp to optimize
its postprocessing pipeline. If you once apply specific steps to a scene, then save it
and reread it from an ASS format using the same post processing settings, they won't
be executed again.

The format comes in two flavours: XML and binary - both of them hold a complete dump of
the 'aiScene' data structure returned by the APIs. The focus for the binary format
(<tt>.assbin</tt>) is fast loading. Optional deflate compression helps reduce file size. The XML
flavour, <tt>.assxml</tt> or simply .xml, is just a plain-to-xml conversion of aiScene.

ASSBIN is Assimp's binary interchange format. assimp_cmd (<tt>&lt;root&gt;/tools/assimp_cmd</tt>) is able to
write it and the core library provides a loader for it.

@section assxml XML File format

The format is pretty much self-explanatory due to its similarity to the in-memory aiScene structure.
With few exceptions, C structures are wrapped in XML elements.

The DTD for ASSXML can be found in <tt>&lt;root&gt;/doc/AssXML_Scheme.xml</tt>. Or have   look
at the output files generated by assimp_cmd.

@section assbin Binary file format

The ASSBIN file format is composed of chunks to represent the hierarchical aiScene data structure.
This makes the format extensible and allows backward-compatibility with future data structure
versions. The <tt>&lt;root&gt;/code/assbin_chunks.h</tt> header contains some magic constants
for use by stand-alone ASSBIN loaders. Also, Assimp's own file writer can be found
in <tt>&lt;root&gt;/tools/assimp_cmd/WriteDumb.cpp</tt> (yes, the 'b' is no typo ...).

@verbatim

-------------------------------------------------------------------------------
1. File structure:
-------------------------------------------------------------------------------

----------------------
| Header (512 bytes) |
----------------------
| Variable chunks    |
----------------------

-------------------------------------------------------------------------------
2. Definitions:
-------------------------------------------------------------------------------

integer is four bytes wide, stored in little-endian byte order.
short   is two bytes wide, stored in little-endian byte order.
byte    is a single byte.
string  is an integer n followed by n UTF-8 characters, not terminated by zero
float   is an IEEE 754 single-precision floating-point value
double  is an IEEE 754 double-precision floating-point value
t[n]    is an array of n elements of type t

-------------------------------------------------------------------------------
2. Header:
-------------------------------------------------------------------------------

byte[44]    Magic identification string for ASSBIN files.
                'ASSIMP.binary'

integer     Major version of the Assimp library which wrote the file
integer     Minor version of the Assimp library which wrote the file
                match these against ASSBIN_VERSION_MAJOR and ASSBIN_VERSION_MINOR

integer     SVN revision of the Assimp library (intended for our internal
            debugging - if you write Ass files from your own APPs, set this value to 0.
integer     Assimp compile flags

short       0 for normal files, 1 for shortened dumps for regression tests
                these should have the file extension assbin.regress

short       1 if the data after the header is compressed with the DEFLATE algorithm,
            0 for uncompressed files.
                   For compressed files, the first integer after the header is
                   always the uncompressed data size

byte[256]   Zero-terminated source file name, UTF-8
byte[128]   Zero-terminated command line parameters passed to assimp_cmd, UTF-8

byte[64]    Reserved for future use
---> Total length: 512 bytes

-------------------------------------------------------------------------------
3. Chunks:
-------------------------------------------------------------------------------

integer     Magic chunk ID (ASSBIN_CHUNK_XXX)
integer     Chunk data length, in bytes
                (unknown chunks are possible, a good reader skips over them)
                (chunk-data-length does not include the first two integers)

byte[n]     chunk-data-length bytes of data, depending on the chunk type

Chunks can contain nested chunks. Nested chunks are ALWAYS at the end of the chunk,
their size is included in chunk-data-length.

The chunk layout for all ASSIMP data structures is derived from their C declarations.
The general 'rule' to get from Assimp headers to the serialized layout is:

   1. POD members (i.e. aiMesh::mPrimitiveTypes, aiMesh::mNumVertices),
        in order of declaration.

   2. Array-members (aiMesh::mFaces, aiMesh::mVertices, aiBone::mWeights),
        in order of declaration.

   2. Object array members (i.e aiMesh::mBones, aiScene::mMeshes) are stored in
      subchunks directly following the data written in 1.) and 2.)


    Of course, there are some exceptions to this general order:

[[aiScene]]

   - The root node holding the scene structure is naturally stored in
     a ASSBIN_CHUNK_AINODE subchunk following 1.) and 2.) (which is
     empty for aiScene).

[[aiMesh]]

   - mTextureCoords and mNumUVComponents are serialized as follows:

   [number of used uv channels times]
       integer mNumUVComponents[n]
       float mTextureCoords[n][3]

       -> more than AI_MAX_TEXCOORD_CHANNELS can be stored. This allows Assimp
       builds with different settings for AI_MAX_TEXCOORD_CHANNELS to exchange
       data.
       -> the on-disk format always uses 3 floats to write UV coordinates.
       If mNumUVComponents[0] is 1, the corresponding mTextureCoords array
       consists of 3 floats.

   - The array member block of aiMesh is prefixed with an integer that specifies
     the kinds of vertex components actually present in the mesh. This is a
     bitwise combination of the ASSBIN_MESH_HAS_xxx constants.

[[aiFace]]

   - mNumIndices is stored as short
   - mIndices are written as short, if aiMesh::mNumVertices<65536

[[aiNode]]

   - mParent is omitted

[[aiLight]]

   - mAttenuationXXX not written if aiLight::mType == aiLightSource_DIRECTIONAL
   - mAngleXXX not written if aiLight::mType != aiLightSource_SPOT

[[aiMaterial]]

   - mNumAllocated is omitted, for obvious reasons :-)


 @endverbatim*/


#define ASSBIN_HEADER_LENGTH 512

// these are the magic chunk identifiers for the binary ASS file format
#define ASSBIN_CHUNK_AICAMERA                   0x1234
#define ASSBIN_CHUNK_AILIGHT                    0x1235
#define ASSBIN_CHUNK_AITEXTURE                  0x1236
#define ASSBIN_CHUNK_AIMESH                     0x1237
#define ASSBIN_CHUNK_AINODEANIM                 0x1238
#define ASSBIN_CHUNK_AISCENE                    0x1239
#define ASSBIN_CHUNK_AIBONE                     0x123a
#define ASSBIN_CHUNK_AIANIMATION                0x123b
#define ASSBIN_CHUNK_AINODE                     0x123c
#define ASSBIN_CHUNK_AIMATERIAL                 0x123d
#define ASSBIN_CHUNK_AIMATERIALPROPERTY         0x123e

#define ASSBIN_MESH_HAS_POSITIONS                   0x1
#define ASSBIN_MESH_HAS_NORMALS                     0x2
#define ASSBIN_MESH_HAS_TANGENTS_AND_BITANGENTS     0x4
#define ASSBIN_MESH_HAS_TEXCOORD_BASE               0x100
#define ASSBIN_MESH_HAS_COLOR_BASE                  0x10000

#define ASSBIN_MESH_HAS_TEXCOORD(n) (ASSBIN_MESH_HAS_TEXCOORD_BASE << n)
#define ASSBIN_MESH_HAS_COLOR(n)    (ASSBIN_MESH_HAS_COLOR_BASE << n)


#endif // INCLUDED_ASSBIN_CHUNKS_H
