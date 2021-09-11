
static int s_var = 5; // cannot be referenced in other cpp files by extern. s_var is encapsulated only this translation unit

//Global variable static == Internal Linkage (locally scoped) + Stored in the static section of memory (not in heap or stack)
//+ Static storgae duration (allocated when the program begins until end) only one instance of the object exists

// s_var can be included into multiple files with a header file->
//each translation will have its own private s_var member during compilation


//static isgood for encapsulation, so that the linker won't pick up duplications across translation units
static void function()
{

}

int t_var = 2; //can be referenced in different cpp files by extern
