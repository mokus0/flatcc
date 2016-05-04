#ifndef CONFIG_H
#define CONFIG_H

#include "flatcc/flatcc_version.h"

/*
 * This is an alternative implementation to googles `flatc` compiler.
 * This configuration enables or disable features which can be used
 * to closer match the flatc behavior. The current flatc version
 * may evolve over time, so the comments on flatc behavior should
 * be seen as how things were when this file was last updated.
 *
 * One notable, but convenient, difference is that structs and enums are
 * not required to be defined in a specific order here. The schema can be
 * rearranged by the user to support the flatc compiler without changing
 * the underlying. This does put an extra restraint on struct depth as
 * recursion must have set maximum level, but this also applies to
 * target C compilers, e.g. clang has a limit of about 100-200
 * nestings.
 *
 * Another more significant difference is that structs are allowed as
 * root objects. This is important if a query should return a partial
 * result from a stored buffer but it isn't currently supported by
 * flatc, although the documentation hints that it might be possible.
 *
 * Value overflows and conversions of enums and default values are
 * checked more than by flatc, and this could break some schemas.
 * However, hopefully it will not break any production schemas since the
 * results would not be well-defined.
 *
 * Struct fields may be deprecated by padding. This isn't supported by
 * flatc but was added because it seemed inconsistent to not have it
 * when deprecated table fields are possible.
 *
 * Enums can be used in structs, but this is not supported by flatc.
 * The reason might be either the additional work in the parser, or
 * because it adds a burden to code generators, for example, C output
 * must place enums before structs. However, they make perfect sense in
 * the format.
 */

#define FLATCC_INFO_TEXT "FlatBuffers schema compiler for C by dvide.com"
#define FLATCC_TITLE_TEXT "flatcc " FLATCC_INFO_TEXT
#define FLATCC_GENERATED_BY "Generated by flatcc " FLATCC_VERSION_TEXT " " FLATCC_INFO_TEXT

/* Include portability layer here since all other files depend on it. */
#ifdef FLATCC_PORTABLE
#include "flatcc/portable/portable.h"
#endif

/*
 * Limit the input buffer or file size being parsed to prevent brining
 * the whole system down on invalid or malicous content. This limit
 * is covers the accumulated size of all included files. 0 is unlimited.
 */
#ifndef FLATCC_MAX_SCHEMA_SIZE
#define FLATCC_MAX_SCHEMA_SIZE 64 * 1024
#endif

/*
 * Max nesting level of includes - the affects recursion on the call
 * stack. 0 is unlimited.
 */
#ifndef FLATCC_MAX_INCLUDE_DEPTH
#define FLATCC_MAX_INCLUDE_DEPTH 100
#endif

/*
 * Max number of include files allowed regardless of how they are
 * nested. This affects memory usage but it may be very large. It should
 * not normally affect the number of open file handles since files are
 * opened and closed individually, depending on the interfacing driver
 * logic. 0 is unlimited.
 */
#ifndef FLATCC_MAX_INCLUDE_COUNT
#define FLATCC_MAX_INCLUDE_COUNT 100
#endif

/*
 * Limit parser recursion depth when analyzing dependent structs.
 * The grammar itself does not contain recursion.
 */
#ifndef FLATCC_NESTING_MAX
#define FLATCC_NESTING_MAX 100
#endif

#ifndef FLATCC_MAX_ERRORS
#define FLATCC_MAX_ERRORS 10
#endif

/* Used when truncating some error messages. */
#ifndef FLATCC_MAX_IDENT_SHOW
#define FLATCC_MAX_IDENT_SHOW 50
#endif

/*
 * Artificial limit on grammar attributes to prevent abuse.
 */
#ifndef FLATCC_ATTR_MAX
#define FLATCC_ATTR_MAX 100
#endif

#ifndef FLATCC_FORCE_ALIGN_MAX
#define FLATCC_FORCE_ALIGN_MAX 256
#endif

/*
 * sizeof(uoffset_t), sizeof(soffset_t) in FlatBuffers file format.
 * Permissible values are 2, 4 (default), and 8.
 */
#ifndef FLATCC_OFFSET_SIZE
#define FLATCC_OFFSET_SIZE 4
#endif

/*
 * sizeof(voffset_t) in FlatBuffers file format.
 * Permissible values are 2 (default), 4, and 8.
 */
#ifndef FLATCC_VOFFSET_SIZE
#define FLATCC_VOFFSET_SIZE 2
#endif

/*
 * DO NOT CHANGE
 * The union type field type.
 * This is hardcoded in many places and difficult
 * to change since size = 1 does not require
 * endian conversion. Config only placed here
 * for consistency and clarity.
 */
#ifndef FLATCC_UTYPE_SIZE
#define FLATCC_UTYPE_SIZE 1
#endif

/*
 * DO NOT CHANGE, see also FLATCC_UTYPE_SIZE
 */
#ifndef FLATCC_BOOL_SIZE
#define FLATCC_BOOL_SIZE 1
#endif

/*
 * There are no hard limits on structs, but
 * they cannot be effectively handled by tables
 * if larger than 64K and 64K-1 allows us to store
 * the size in short where needed. Internally
 * we can handle 64 bits.
 */
#ifndef FLATCC_STRUCT_MAX_SIZE
#define FLATCC_STRUCT_MAX_SIZE ((1 << 16) - 1)
#endif

#ifndef FLATCC_NAMESPACE_MAX
#define FLATCC_NAMESPACE_MAX 100
#endif

/* Reserve space for trailing _ and \0. */
#define FLATCC_NAMESPACE_BUFSIZ (FLATCC_NAMESPACE_MAX + 2)
#define FLATCC_NAME_BUFSIZ (2 * FLATCC_NAMESPACE_BUFSIZ)

#ifndef FLATCC_FILENAME_MAX
#define FLATCC_FILENAME_MAX 256
#endif

/*
 * If set, enum fields initialized with a numeric constant must match an
 * existing enum value. This only applies to the schema intializers, not
 * values assigned during buffer construction. It may be useful to allow
 * non-existing values (and expensive to enforce), but for schema
 * intializers, it could easily break future changes if inconsistent
 * values are being used.
 */
#ifndef FLATCC_STRICT_ENUM_INIT
#define FLATCC_STRICT_ENUM_INIT 1
#endif

/*
 * flatc requires ascending enums, and it does simplify code generation
 * to multiple languages, and name mapping of enums, so we require this
 * by default, but C code generator handles both cases fine.
 */
#ifndef FLATCC_ASCENDING_ENUM
#define FLATCC_ASCENDING_ENUM 1
#endif

/*
 * flatc tool accepts any integer as bool because it
 * doesn't do range check. By enabling this flag we get more compatible,
 * but still only accepts 1 or 0 as bool, and also allow assignment of
 * bool to other types (useful in enums that are not bool because
 * official flatc has no boolean enum type.
 *
 * However, there appears to be no strong reason for mixing boolean types
 * with other types, but since `flatc` supports, it is enabled by
 * default.
 */
#ifndef FLATCC_ALLOW_BOOLEAN_CONVERSION
#define FLATCC_ALLOW_BOOLEAN_CONVERSION 1
#endif

/* flatc does not recognize later defined enums, be we do by default. */
#ifndef FLATCC_HIDE_LATER_ENUM
#define FLATCC_HIDE_LATER_ENUM 0
#endif

/*
 * The flatc parser does not recognize later structs.
 * If disabled dependent structs will be topologically
 * sorted to the set maximum recursion depth.
 */
#ifndef FLATCC_HIDE_LATER_STRUCT
#define FLATCC_HIDE_LATER_STRUCT 0
#endif

/*
 * In all cases we permit at most one root type per file.
 * flatc allows none, or multiple, but apparently only generates
 * accessors for the last seen.
 */
#ifndef FLATCC_REQUIRE_ROOT_TYPE
#define FLATCC_REQUIRE_ROOT_TYPE 0
#endif

/*
 * This also applies to nested_flatbuffers.
 * flatc does not allow structs.
 */
#ifndef FLATCC_ALLOW_STRUCT_ROOT
#define FLATCC_ALLOW_STRUCT_ROOT 1
#endif

/*
 * pad deprecated struct fields with __depracedx[size] i C output.
 * flatc does not permit deprecated struct fields.
 */
#ifndef FLATCC_ALLOW_STRUCT_FIELD_DEPRECATE
#define FLATCC_ALLOW_STRUCT_FIELD_DEPRECATE 1
#endif

/*
 * flatc only allows one key on table fields, which makes sense in
 * C++ STL where a vector can have on comparison operator,
 * but in C output we can easily have multiple <structname>_find_by_<fieldname>.
 * Vectors can only be sorted by one field at a time, but e.g. an
 * octree- or trie-like data-structure might choose to sort on different
 * coordinates at different levels.
 */
#ifndef FLATCC_ALLOW_MULTIPLE_KEY_FIELDS
#define FLATCC_ALLOW_MULTIPLE_KEY_FIELDS 1
#endif

/* flatc currently does not allow keys on structs, but it makes sense to have them. */
#ifndef FLATCC_ALLOW_STRUCT_FIELD_KEY
#define FLATCC_ALLOW_STRUCT_FIELD_KEY 1
#endif

/*
 * Structs cannot have defaults. If an enum does not define a 0 value
 * this is not considered an error when used with a struct.
 * The flatc compiler does not accept enums on structs.
 */
#ifndef FLATCC_ALLOW_ENUM_STRUCT_FIELD
#define FLATCC_ALLOW_ENUM_STRUCT_FIELD 1
#endif

/*
 * flatc currrently only allows scalar and strings as keys,
 * but enums are really a kind of scalar and it makes sense to support
 * them.
 */
#ifndef FLATCC_ALLOW_ENUM_KEY
#define FLATCC_ALLOW_ENUM_KEY 1
#endif

/*
 * String keys are allowed (and actively used in refletion.fbs) by
 * flatc, but they are more tricky to support in code generation
 * so allow them to be disabled.
 */
#ifndef FLATCC_ALLOW_STRING_KEY
#define FLATCC_ALLOW_STRING_KEY 1
#endif

/* Code gen specific. */

#ifndef FLATCC_DEFAULT_SCHEMA_EXT
#define FLATCC_DEFAULT_SCHEMA_EXT ".fbs"
#endif

#ifndef FLATCC_DEFAULT_BIN_SCHEMA_EXT
#define FLATCC_DEFAULT_BIN_SCHEMA_EXT ".bfbs"
#endif

#ifndef FLATCC_DEFAULT_BIN_EXT
#define FLATCC_DEFAULT_BIN_EXT ".bin"
#endif

#ifndef FLATCC_DEFAULT_FILENAME
#define FLATCC_DEFAULT_FILENAME "test"
#endif

#ifndef FLATCC_DEFAULT_NAMESPACE
#define FLATCC_DEFAULT_NAMESPACE ""
#endif

#ifndef FLATCC_DEFAULT_NAMESPACE_COMMON
#define FLATCC_DEFAULT_NAMESPACE_COMMON "flatbuffers"
#endif

/* rpc attributes aren't spec'ed at this point, so we flag the code. */
/* Similar to attributes after table name (no runtime option). */
#ifndef FLATCC_ALLOW_RPC_SERVICE_ATTRIBUTES
#define FLATCC_ALLOW_RPC_ATTRIBUTES 1
#endif
 
/* Similar to attributes after a tables field name (no runtime option). */
#ifndef FLATCC_ALLOW_RPC_METHOD_ATTRIBUTES
#define FLATCC_ALLOW_RPC_METHOD_ATTRIBUTES 1
#endif

/* Only takes effect if rpc method attributes are allowed (no runtime option). */
#ifndef FLATCC_ALLOW_DEPRECATED_RPC_METHOD
#define FLATCC_ALLOW_DEPRECATED_RPC_METHOD 1
#endif


/*
 * This inserts #pragma pack(1) and padding fields instead
 * of using alignas on field members.
 */
#define FLATCC_CGEN_PAD 0


/* ---- CGEN are flagss specific the C code generator. ---- */

/*
 * Generate support code for sorting arrays.
 */
#ifndef FLATCC_CGEN_SORT
#define FLATCC_CGEN_SORT 1
#endif

/* Disable warnings for known compilers */
#ifndef FLATCC_CGEN_PRAGMAS
#define FLATCC_CGEN_PRAGMAS 1
#endif

/* Default spacing when generating auto idented code. */
#ifndef FLATCC_CGEN_SPACING
#define FLATCC_CGEN_SPACING 4
#endif


/* ---- BGEN are flags specific to the binary schema generator. ---- */

/*
 * If disabled, no binary schema support is compiled in.
 * This may be useful when generection reflection headers
 * are no longer compatible with the current build. Disabling
 * reflection makes it possible to still build the compiler and
 * thus generate new reflection headers.
 *
 * This flag is enabled by the build system so files can also be
 * excluded or included.
 *
 * #ifndef FLATCC_REFLECTION
 * #define FLATCC_REFLECTION 0
 * #endif
*/

/* Export option for --schema. */
#ifndef FLATCC_BGEN_BFBS
#define FLATCC_BGEN_BFBS 0
#endif

/*
 * Add uoffset length field before buffer. This is not a valid buffer
 * then but can be used to concatenate multiple files and it is
 * compatible with the layout of nested buffers that have a similar
 * prefix. The prefix is a uoffset_t field.
 */
#ifndef FLATCC_BGEN_LENGTH_PREFIX
#define FLATCC_BGEN_LENGTH_PREFIX 0
#endif

/* Prefix type names with namespace during schema export.
 * flatc does not include the namespace prefix. */
#ifndef FLATCC_BGEN_QUALIFY_NAMES
#define FLATCC_BGEN_QUALIFY_NAMES 1
#endif


/* ---- JSON related code generation. ---- */

/*
 * Translate enum values to symbolic names. Ditto union types.
 * This is also a runtime option but disabling it here uses
 * a faster code path.
 */
#ifndef FLATCC_JSON_PRINT_MAP_ENUMS
#define FLATCC_JSON_PRINT_MAP_ENUMS 1
#endif

#endif /* CONFIG_H */
