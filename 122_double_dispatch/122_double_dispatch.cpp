/*
read 121 static_dynamic_dispatch first
https://dzone.com/articles/double-dispatch-in-c
http://www.vishalchovatiya.com/double-dispatch-in-cpp/
https://gieseanw.wordpress.com/2018/12/29/stop-reimplementing-the-virtual-table-and-start-using-double-dispatch/#more-1692
*/

#include <iostream>

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

        /*The Problem:
        
        compiler can't dispatch (link) the call  "rock.collide_with(vehicle_ref)"
        to the collide_with that takes in Lamborghini& (the run-time type of vehicle)
        as we expect, because dispatching (func overload resolution, a.k.a compile-time polymorphism)
        occurs only at compile-time, while the true identity (run-time type) of  "vehicle" can only
        be deduced at run-time when the expression is used. Therefore the compiler links
        the call to the function that simply accepts "Vehicle&" which is the underlying
        type of the reference, known at compile time (cpp is statically typed language).

        At this point we have 3 options to decipher the true identity (run-time type) of "vehicle"
        all will execute at run-time: 

        1. check if(dynamic_cast<Lamborghini&>(vehicle)) and then call "collide_with_lamborghini(vehicle)"
        or if(dynamic_cast<Mercedez&>(vehicle)) and then call "collide_with_mercedez(vehicle)"
        *dynamic_cast utilizes RTTI (run-time type information) a mechanism that allows
        the type of an object to be determined during program execution (performance hit)

        2. "typeid(*vehicle) == typeid(Lamborghini)" or  "typeid(*vehicle) == typeid(Lamborghini)"
        three times faster than rtti, as seen in 1 above

        *options 1 and 2 are slow and require potentially endless if-else branching

        3. double dispatch - meachnism that can execute the correct function at run time,
        from a set of overloaded functions, based on the run-time type of a variable.
        This mechanism is based on 2 main function calls (dispatches), where in the
        1st we actually invoke a call through the polymorphic ptr/rf  (dynamic dispatch)
        which leads us to the class that represents the run-time type of the object
        (revealing the type!!!!!) and in the 2nd dispatch we use the revealed type for our needs.

        
        the following statement is the 1st dispatch:
        when executing this statement and using "vehicle" expression, 
        cpp will decipher the run-time type of vehicle (Lamborghini),
        therefore the "rock_visiting_vehicle" func of Lamborghini will be invoked.
        only then we will be aware of the true identity of vehicle - therefore
        we can utilize this information as seen in the 2nd dispatch
        
        *rock visits lamborghini meaning we pass rock to the "rock_visiting_vehicle"
        of Lamborghini.

        Important! vehicle_ref IS NOT SLICED TO vehicle in this function,
        it retains the run-time type of Lamborghini!!!
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
    /*
    Double dispatch - 
    
    A mechanism for invoking the correct method from a set of overloaded methods
    based on the run-time type of a polymorphic ptr/ref to an instance.
    works by invoking 2 function calls:
    1. Deduce the run-time type of the ptr/ref: 
    in the first call we invoke a function via the polymorphic ptr/ref - using
    it will decipher its run-time type, calling the method from the class that the ptr/ref
    points/refers to.
    2. Using *this:
    once the run-time type is deciphered, we can pass back the deciphered type
    using "*this" to the calling class.

    a practical exmample is seen next.
    
    */
    Lamborghini lamborghini;
    Vehicle& vehicle_ref = lamborghini;
    Rock rock;
    rock.collide_with(vehicle_ref);

    /*
    Note: double dispatch is a good solution but the problem with is that RockVisitor
    holds a function for lamborghini and mercedez. 122a_crtp shows how we can have 
    a single visit function and a template parameter type T (which denotes if the
    incoming parameter is a lamborghini or merceded, enabling us to cast the "this" (vehicle)
    to the parameter T. 
    */
}



