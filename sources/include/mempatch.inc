#if defined _mempatch_included
 #endinput
#endif
#define _mempatch_included

enum MemPatchType
{
	MEM_PATCH_BYTES = 0,
	MEM_PATCH_INT8 = 1,
	MEM_PATCH_UINT8 = 2,
	MEM_PATCH_INT16 = 3,
	MEM_PATCH_UINT16 = 4,
	MEM_PATCH_INT32 = 5,
	MEM_PATCH_UINT32 = 6,
	MEM_PATCH_FLOAT = 7,
	MEM_PATCH_DOUBLE = 8,
	MEM_PATCH_STRING = 9
};

/*
 * Setups a new bytes patch
 *
 * @param gameconf	Handle to gamedata
 * @param sig		Signature string in the gamedata to use
 * @param offset	Offset string in the gamedata to use
 * @param patch		Patch string in gamedata to use Note: Must be a key value, and the key name must be followed by the OS Example MemPatch_Windows, MemPatch_Linux. MemPatch would be passed.
 *
 * @return Handle to the patch
*/
native Handle:SetupMemoryPatchBytes(Handle:gameconf, const String:sig[], const String:offset[], const String:patch[]);

/*
 * Setups a new int patch
 *
 * @param gameconf	Handle to gamedata
 * @param sig		Signature string in the gamedata to use
 * @param offset	Offset string in the gamedata to use
 * @param type		Size of int and type (unsigned/signed)
 *
 * @return Handle to the patch
*/
native Handle:SetupMemoryPatchInt(Handle:gameconf, const String:sig[], const String:offset[], MemPatchType:type=MEM_PATCH_INT32);

/*
 * Setups a new float patch
 *
 * @param gameconf	Handle to gamedata
 * @param sig		Signature string in the gamedata to use
 * @param offset	Offset string in the gamedata to use
 * @param type		Size of float (float/double)
 *
 * @return Handle to the patch
*/
native Handle:SetupMemoryPatchFloat(Handle:gameconf, const String:sig[], const String:offset[], MemPatchType:type=MEM_PATCH_FLOAT);

/*
 * Setups a new string patch
 *
 * @param gameconf	Handle to gamedata
 * @param sig		Signature string in the gamedata to use
 * @param offset	Offset string in the gamedata to use
 * @param size		Size of the string being patched
 *
 * @return Handle to the patch
*/
native Handle:SetupMemoryPatchString(Handle:gameconf, const String:sig[], const String:offset[], size);

/*
 * Reads the current int at the memory location used by the patch
 *
 * @param mempatch	Handle to memory patch to get value for
 *
 * @return Int value of the patch location
*/
native ReadMemoryInt(Handle:mempatch);

/*
 * Reads the current float at the memory location used by the patch
 *
 * @param mempatch	Handle to memory patch to get value for
 *
 * @return Float value of the patch location
*/
native Float:ReadMemoryFloat(Handle:mempatch);

/*
 * Reads the current string at the memory location used by the patch
 *
 * @param mempatch	Handle to memory patch to get value for
 * @param buffer	Buffer to store the return string
 * @param buffersize	Size of buffer
 *
 * @return bytes copied to buffer
*/
native ReadMemoryString(Handle:mempatch, String:buffer[], buffersize);

/*
 * Reads a number of bytes and returns and opcode string
 *
 * @param gameconf		Handle to gamedata file
 * @param sig			Signature string in the gamedata to use
 * @param offset		Offset string in the gamedata to use
 * @param size			Number of bytes to read
 * @param buffer		Buffer to store the opcodes
 * @param buffersize	Size of buffer
 *
 * @noreturn
*/
native ReadMemoryBytes(Handle:gameconf, const String:sig[], const String:offset[], size, const String:buffer[], buffersize);

/*
 * Patches the int at the memory location used by the patch
 *
 * @param mempatch	Handle to memory patch to get value for
 * @param value		Value to set memory location to
 *
 * @noreturn
*/

native MemoryPatchInt(Handle:mempatch, value);

/*
 * Patches the float at the memory location used by the patch
 *
 * @param mempatch	Handle to memory patch to get value for
 * @param value		Value to set memory location to
 *
 * @noreturn
*/
native MemoryPatchFloat(Handle:mempatch, Float:value);

/*
 * Patches the string at the memory location used by the patch
 *
 * @param mempatch	Handle to memory patch to get value for
 * @param value		Value to set memory location to
 *
 * @noreturn
*/
native MemoryPatchString(Handle:mempatch, const String:value[]);

/*
 * Patches bytes to the patch provided on setup
 *
 * @param mempatch	Handle to memory patch to get value for
 *
 * @noreturn
*/
native MemoryPatchBytes(Handle:mempatch);

/*
 * Restores a patch to the value gather on setup
 *
 * @param mempatch	Handle to memory patch to restore
 *
 * @noreturn
*/
native RestoreMemoryPatch(Handle:mempatch);

/*
 * Returns an array of byte patter offsets
 * 
 * @param gameconf	Handle to gamedata file
 * @param sig		Signature string in the gamedata to use
 * @param offset	Offset to start from 
 * @param bytes		Number of bytes to search through to find the patter
 * @param match		Bytes pattern to match
 * @param matches	Array to store offsets where match occured
 * @param arraysize	Size of array being passed for storing matches
 *
 * @return matches found
*/
native GetBytePatternOffsets(Handle:gameconf, const String:sig[], offset, bytes, const String:match[], matches[], arraysize);

/*
 * Returns an array of byte patter offsets
 * 
 * @param gameconf	Handle to gamedata file
 * @param sig		Signature string in the gamedata to use
 * @param offset	Offset to start from 
 * @param type		Type of patch to do
 * @param size		Used if the patch type is a string else leave as 0
 * @param patch		Used if patch type is BYTES patch else leave empty
 *
 * @return mempatch handle
*/
native Handle:SetupOffsetMemoryPatch(Handle:gameconf, const String:sig[], offset, MemPatchType:type=MEM_PATCH_BYTES, size = 0, const String:patch[] = "");

public Extension:__ext_mempatch =
{
	name = "mempatch",
	file = "mempatch.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_EXTENSIONS
public __ext_mempatch_SetNTVOptional()
{
	MarkNativeAsOptional("SetupMemoryPatchBytes");
	MarkNativeAsOptional("SetupMemoryPatchInt");
	MarkNativeAsOptional("SetupMemoryPatchFloat");
	MarkNativeAsOptional("SetupMemoryPatchString");
	MarkNativeAsOptional("ReadMemoryInt");
	MarkNativeAsOptional("ReadMemoryFloat");
	MarkNativeAsOptional("ReadMemoryString");
	MarkNativeAsOptional("ReadMemoryBytes");
	MarkNativeAsOptional("MemoryPatchInt");
	MarkNativeAsOptional("MemoryPatchFloat");
	MarkNativeAsOptional("MemoryPatchString");
	MarkNativeAsOptional("MemoryPatchBytes");
	MarkNativeAsOptional("RestoreMemoryPatch");
	MarkNativeAsOptional("GetBytePatternOffsets");
	MarkNativeAsOptional("SetupOffsetMemoryPatch");
}
#endif