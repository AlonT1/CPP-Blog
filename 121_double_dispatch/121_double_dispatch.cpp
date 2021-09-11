#include <iostream>

class SpaceShip {};
class ApolloSpacecraft : public SpaceShip {};

class Asteroid 
{
public:
    virtual void collide_with(SpaceShip&) { std::cout << "Asteroid hit a SpaceShip\n"; }
    virtual void collide_with(ApolloSpacecraft&) { std::cout << "Asteroid hit an ApolloSpacecraft\n";}
};

class ExplodingAsteroid : public Asteroid 
{
public:
    virtual void collide_with(SpaceShip&) override { std::cout << "ExplodingAsteroid hit a SpaceShip\n"; }
    virtual void collide_with(ApolloSpacecraft&) override { std::cout << "ExplodingAsteroid hit an ApolloSpacecraft\n";}
};




/***************Solution - Double dispatch*******************/
  /*
  vehicle_ref can refere to either Mercedez or Lamborghini, either way
  the underlying type of vehicle_Ref (Vehicle&) will be chosen
  in the following statement since function overloading works at compile-time
  and the type that vehicle_ref refers to can only be deduced at run-time:

  Vehicle& vehicle_ref = lamborghini;
  rock.collide_with(vehicle_ref); // Vechile& is chose

  rock holds collide_with(Mercedez&) and collide_with(Lamborghini&), both will fail
  since vehicle_ref is deduced as Vehicle&, and we of course want it to be deduced
  as a Lamborghini.

  Solution - role reversal - since the argument isn't sliced and we know the
  type of the object it refers to - the argument will be the one who performs the requested function:
  although we cannot choose a function based on a polymorphic ptr/ref arguemnt during
  function overloading, we utilize the same polymorphic ptr/ref in non-overloaded
  function:

  rock.collide_with(vehicle_ref);

  since we know that vehicle_ref refers to a lamobrghini (vehicle_ref isn't sliced)
  we can simply invoke "vehicle_ref.collide_with(this)"
  now lamborghini accepts the rock with its collide_with(rock) function,
  in other words rock will VISIT labmorghini.

  The main issue is that instead of the rock handling collide_with(x),
  we actually reversed the roles, now "x" performs the collision - simply
  because of the fact that we know who "x" actually refers to, and this information
  cannot be utilized during function overloading. in other words,
  the implementation of collide_with is at the wrong logical place, we want
  Rock to responsible of the collide_with implementation, and not delegate and scatter
  it aroung other classes, we want Rock to have something like:
  collide_with(Mercedez&) and collide_with(Lamborghini&) and thats it.

  */
class Mercedez;
class Lamborghini;
class Rock;
class RockVisitor;

class RockVisitor
{
public:
    Rock* rock = nullptr;
    void vehicle_visiting_rock(Lamborghini& lamborghini);
    void vehicle_visiting_rock(Mercedez& mercedez);
};

class Vehicle
{
public:
    virtual void rock_visiting_vehicle(RockVisitor& rock_visitor) = 0;
};

class Mercedez : public Vehicle
{
    void rock_visiting_vehicle(RockVisitor& rock_visitor)
    {
        //2nd dispatch - now we possess the chosen vehicle - use rock_visitor
        //to invoke the correct collide_with() function in rock_visitor
        //we CAN utilize function overloading because we are sure about the type
        //of the vehicle (Mercedez)
        rock_visitor.vehicle_visiting_rock(*this); 
    }
};

class Lamborghini : public Vehicle
{
public:
    void rock_visiting_vehicle(RockVisitor& rock_visitor)
    {
        //2nd dispatch: in the 1st dispatch that was executed in real-time, cpp
        //has deciphered "vehicle" as a reference to Lamborghini, therefore we are
        //here now and we know for sure that vehicle refers to Lamborghini.
        //now the compiler can utilize function overloading, since "*this"
        //isn't polymorphic  and doesn't require run-time querying (RTTI),
        //because we know that *this is always a Lamborghini, thus
        //during compile-time the function below was statically dispatched
        //(linked to the implementation) to "vehicle_visiting_rock(Lamborghini&)",
        //which in turn will invoke collide_with(lamborghini) of rock.
        //*to communicate with our rock, rock_visitor holds a pointer to rock
        rock_visitor.vehicle_visiting_rock(*this); //2nd dispatch back to rock
    }
};


class Rock
{
public:
    void collide_with(Vehicle& vehicle)
    {   
        RockVisitor rock_visitor{ this }; //holds a ptr to the current Rock
        /*although vehicle refers to Lamborghini, since function
        overloading only works at compile-time, the 2 functions below cannot be
        chosen. Therefore this collide_with(Vehicle&) is chosen at run-time via dynamic
        dispatch.
        at this point we have 3 options: 
        1. check if(dynamic_cast<Lamborghini&>(vehicle)) collide_with_lamborghini(vehicle)
        or if(dynamic_cast<Mercedez&>(vehicle)) collide_with_mercedez(vehicle)
        *dynamic_cast utilizes RTTI (run-time type information) a mechanism that allows
        the type of an object to be determined during program execution (performance hit)

        2. "typeid(vehicle) == typeid(Lamborghini)" or  "typeid(vehicle) == typeid(Lamborghini)"
        three times faster than rtti, as seen in 1 above

        *options 1 and 2 are slow and require potentially endless if-else branching

        3. double dispatch - meachnism that can execute the correct function,
        from a set of overloaded functions, based based on the run-time type of a variable.
        This mechanism is based on 2 main function calls (dispatches).
        As mentioned, since run-time type of vehicle is deduced at run-time, function
        overloading which occurs at compile-tie cannot be utilized based on
        the run-time type of vehicle(Lamborghini), thus the type of the reference which
        is known at compile time (Vehicle&) is utilized to pick the function (cpp is statically typed).
        
        the following statement is the 1st dispatch:
        in the following statement, cpp will decipher the run-time type of vehicle (Lamborghini),
        therefore the "rock_visiting_vehicle" func of Lamborghini will be invoked.
        Once we are at the Lamborghini class - the true identity of vehicle is revealed
        and we utilize this information as seen in the 2nd dispatch
        
        *rock visits lamborghini meaning we pass rock to the "rock_visiting_vehicle"
        of Lamborghini.
        */
	    vehicle.rock_visiting_vehicle(rock_visitor);
    }

    void collide_with_lamborghini(Lamborghini& lamborghini)
    {
        std::cout << "collided with lamborghini \n";
    }

    void collide_with_mercedez(Mercedez& mercedez)
    {
        std::cout << "collided with mercedez \n";
    }
};

void RockVisitor::vehicle_visiting_rock(Lamborghini& lamborghini)
{
    rock->collide_with_lamborghini(lamborghini);
}

void RockVisitor::vehicle_visiting_rock(Mercedez& mercedez)
{
    rock->collide_with_mercedez(mercedez);
}


int main()
{
    /****************Testing function overloading (static dispatch - at compile time)*******************/
    //spaceships
    SpaceShip space_ship;
    ApolloSpacecraft apollo_spacecraft;

    //asteroid
    Asteroid asteroid;
    asteroid.collide_with(space_ship); // invokes Asteroid::collide_with(SpaceShip&) as expected
    asteroid.collide_with(apollo_spacecraft); // invokes Asteroid::collide_with(ApolloSpacecraft&) as expected

    //Exploding Asteroid
    ExplodingAsteroid exploding_asteroid;
    exploding_asteroid.collide_with(space_ship); // invokes ExplodingAsteroid::collide_with(SpaceShip&) as expected
    exploding_asteroid.collide_with(apollo_spacecraft); //invokes ExplodingAsteroid::collide_with(ApolloSpacecraft&) as expected

    std::cout << "\n";


    /***************Dynamic dispatch - works*******************/
    Asteroid& asteroid_ref = exploding_asteroid;
    asteroid_ref.collide_with(space_ship);
    asteroid_ref.collide_with(apollo_spacecraft);

    std::cout << "\n";
    

    /***************The problem is here!!!!!!!!!!!!!!!!!!!!!!!!******************
    tl;dr polymorphic references underogo slicing when used in function overloading.

    function overloading mechanism works at compile-time, thus functions cannot
    be chosen based on the type of an argument that a polymorphic reference refers to since
    his type can only be deduced at run-time in the context of dynamic dispatch!
    therefore the compiler selectes the underyling type of the reference argument
    (spaceship_ref arguemnt below is chosen to be type of SpaceShip&, and not the run-time 
    type ApolloSpaceCraft which is the type of the object that vehicle refers to).
    Note that this isn't "slicing", the spaceship_ref is still an alias for 
    a ApolloSpaceCraft object (stores both Spaceship and ApolloSpaceCraft sections),
    but simply we cannot choose a function based on the referred type, therefore a function
    that supports Spaceship& will be chosen.
    */

    SpaceShip& spaceship_ref = apollo_spacecraft; //although spaceship_ref refers to an ApolloSpaceCraft object......
    asteroid.collide_with(spaceship_ref); // (1) Asteroid collides with SpaceShip and not with ApolloSpaceCraft!!!
    asteroid_ref.collide_with(spaceship_ref); //(2) ExplodingAsteroid collides with SpaceShip and not with ApolloSpaceCraft!!!

    //note: (1) is invokes via static dispatch, and (2) invokes dynamic dispatch (explained more below).
    //this goes to show that no matter if we invoke the collide_with function through an instance variable (1)
    //or through a polymoprhic reference (2), in both cases collide_with must undergo compile-time function overloading
    //and it can't accept a polymorphic ref which the type it refers to can only be deduced at run-time, which
    //stands in contrast to the compile-time aspect of function overloading.

    std::cout << "\n";


  

    Lamborghini lamborghini;
    Vehicle& vehicle_ref = lamborghini;
    Rock rock;
    rock.collide_with(vehicle_ref);



    
}




/* Further info about static dispatch and dynamic dispatch - out of the scope of "double_dispatch":


    1. "asteroid.collide_with(spaceship_ref);"  - undergoes static dispatch:
    asteroid is variable instance on the stack
    this "violates" one of the 3 conditions that enable dynamic dispatch: "asteroid"
    isn't a polymorphic ref/ptr (see 28_virtual_functions) - therefore static dispatch will occur..

    During the compilation and as a part of static dispatch, the compiler peforms name-mangling
    to functions - this helps to differ between them without any vtable mechanism, even if they
    are overload each other (with the same name - but with different params.
    diff return type don't affect function overlading - and probably not name mangling,
    meaning that 2 functions with same name + params but different ret types is not enought
    for the compiler to perform function overload and differentiate between them).
    For example:
    collide_with(SpaceShip&) becomes something like ___collide_with__spaceship
    collide_with(ApolloSpaceCraft&) becomes something like ___collide_with__apollospacecraft.
    and becuase we perform static dispatch, spaceship_ref is sliced to SpaceShip and the first function is chosen.


    2. "asteroid_ref.collide_with(spaceship_ref);" - undergoes dynamic dispatch:
    dynamic dispatch is chosen here due
    to the fact that all 3 conditions for dynamic dispatch coexist:
    a. inheritance (ExplodingAsteroid inherits from Asteroid)
    b. virtual function override - ExplodingAsteroid overrides Asteroid's collide_with virtual function
    c. polymorphic pointer / reference - a reference of type Base (Asteroid) represents/refers a
    sub-class (ExplodingAsteroid).
    https://gieseanw.wordpress.com/2018/12/29/stop-reimplementing-the-virtual-table-and-start-using-double-dispatch/#more-1692
    http://www.vishalchovatiya.com/double-dispatch-in-cpp/
    */