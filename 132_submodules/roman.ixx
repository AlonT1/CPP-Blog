// roman.ixx – Module interface file of the roman module
export module roman; // due to "export module " the module becomes a module interface file, "importable" by others
// all entities that are marked as "export" are reachable by the importing (consuming) this module
export import roman.from; //we import the module and then "export" it 
export import roman.to;