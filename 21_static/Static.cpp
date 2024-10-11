
static int s_var = 5; // cannot be referenced in other cpp files by extern. s_var is encapsulated only this translation unit

//Global variable static == Internal Linkage (locally scoped) + Stored in the static section of memory (not in heap or stack)
//+ Static storgae duration (allocated when the program begins until end) only one instance of the object exists

// s_var can be included into multiple files with a header file->
//each translation will have its own private s_var member during compilation


/*
 static function -> scope limit to current translation unit by applying internal linkage
 Without static, a function has a external linkage,--> translation units (other .cpp files in the project)
 could access the function. If two or more files define functions with the same name and do not use static,
 the linker will encounter multiple definitions and raise an error.

 Note that inline functions do not apply internal linkage. inline does not violate ODR - same inline function can be
 defined in multiple files - possible because the code itself is simply copied, no real function will exist at compile-time.
*/
static void function()
{

}

int t_var = 2; //can be referenced in different cpp files by extern
