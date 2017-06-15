/*
Theodore Chang
N10815055
Sec A
Hw 05 inheritance
*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Protector{
public:
	Protector(const string& theName, int theStrength) : name(theName), strength(theStrength), alive(true){}
	
	virtual void state() const =0; //pure virtual object
	virtual void setStrength(int theStrength){strength = theStrength;}
	virtual int getStrength() const {return strength;}
	virtual string getName() const { return name; }
	virtual bool needName() const { return false; }
	virtual bool living() const { return alive; }
	virtual void dead() {
		alive = false;
		strength = 0;
	}
private:
	string name;
	unsigned strength;
	bool alive;
};

class Wizard : public Protector{
public:
	Wizard(const string& theName, int theStrength) : Protector(theName, theStrength) {}
	//using Protector::Protector;
	void state() const {
		cout << "POOF\n";
	}
private:
};

class Warrior : public Protector{
public:
	Warrior(const string& theName, int theStrength) : Protector(theName, theStrength) {}
	//using Protector::Protector;
	void state() const{}
private:
};

class Archer : public Warrior{
public:
	Archer(const string& theName, int theStrength) : Warrior(theName, theStrength) {}
	//using Protector::Protector;
	void state() const{
		cout << "TWANG! " << getName() << " says: Take that in the name of my lord, "  /*<< getLordName()*/ ;
		//"TWANG! <archer's name> says: Take that in the name of my lord, __________" (whence he shouts the name of the lord he is sworn to defend)
	}
	bool needName() const {return true;}
private:
};

class Swordsman : public Warrior{
public:
	Swordsman(const string& theName, int theStrength) : Warrior(theName, theStrength) {}
	//using Protector::Protector;
	void state() const{
		cout << "CLANG! " << getName() << " says: Take that in the name of my lord, " /*<< getLordName()*/ ;
		//"CLANG! <swordsman's name>says: Take that in the name of my lord, __________"
	}
	bool needName() const { return true; }
private:
};

class Noble{
public:
	Noble(const string& theName) : name(theName) {}
	virtual string getName() const { return name; }
	virtual unsigned getStrength() const = 0;
	virtual void takeDamage(int damage) {};
	virtual void dead() = 0;
	virtual void display() const = 0;
	virtual void battle(Noble& theNoble){
		cout << name << " battles " << theNoble.getName() << endl;//narrative
		if (getStrength() > theNoble.getStrength()){//when one noble is more powerful 
			
			int damage = getStrength() - theNoble.getStrength();
			takeDamage(damage);//deducting the damage from the winning noble
			theNoble.dead();//setting the losing noble's warriors' strengths to 0
			cout << name << " defeats " << theNoble.getName() << endl;
		}
		else if (getStrength() < theNoble.getStrength()){
			
			int damage = theNoble.getStrength() - getStrength();
			theNoble.takeDamage(damage);//deducting the damage from the winning noble
			dead();//setting the losing noble's warriors' strengths to 0
			cout << theNoble.getName() << " defeats " << name << endl;
		}
		else {//when the two nobles have the same strength
			
			dead();
			theNoble.dead();
			cout << "Mutual Annihalation: " << name << " and " << theNoble.getName() << " die at each other's hands" << endl;
		}
	}
private:
	string name;
};

class PersonWithStrengthToFight : public Noble {
public:
	PersonWithStrengthToFight(const string& theName, int theStrength) : Noble(theName) , strength(theStrength){}
	unsigned getStrength() const {return strength;}
	void dead() {	strength = 0;}
	void display() const { cout << getName() << " " << getStrength() << endl; }
	void takeDamage(int damage){strength -= damage;}
private:
	unsigned strength; //this object has strength 
};

class Lord : public Noble{
public:
	Lord(const string& theName) : Noble(theName), alive(true) {}
	unsigned getStrength() const {//get strength. sum of the protectors' strength
		unsigned strength = 0;
		for (size_t i = 0; i < protectors.size(); ++i){
			strength = strength + protectors[i]->getStrength();
		}
		return strength;
	}
	void state() const{ //the living protectors state. getName() aid for Swordsman and Archers
		for (size_t i = 0; i < protectors.size(); ++i){
			if (protectors[i]->living() == true){
				protectors[i]->state();
				if (protectors[i]->needName() == true){
					cout << getName() << endl;
				}
			}
		}
	}
	void dead() { //dead. all protectors strengths to 0
		state();
		alive = false;
		for (size_t i = 0; i < protectors.size(); ++i){
			protectors[i]->dead();
		}
	}
	void display() const { //display name, strength, and the army 
		cout << getName() << " " << getStrength() << " Army of " << protectors.size()<< endl;
		for (size_t i = 0; i < protectors.size(); ++i){
			cout << "   " << protectors[i]->getName() << " "<< protectors[i]->getStrength() << endl;
		}
	}
	void takeDamage(int damage){ //subtracting damage from the army
		state();
		for (size_t i = 0; i < protectors.size(); ++i){//deducting the damage from the winning noble
			if (damage > protectors[i]->getStrength()){
				damage -= protectors[i]->getStrength();
				protectors[i]->dead();
			}
			else{
				protectors[i]->setStrength(protectors[i]->getStrength() - damage);
				damage = 0;
				break;
			}
		}
	}
	void hires(Protector& theProtector){ //add protector to the collection
		if ((alive = true) && (theProtector.living() == true))
			protectors.push_back(&theProtector);
	}
private:
	vector<Protector*> protectors;
	bool alive;
};



int main() {

	Lord sam("Sam");
	Archer samantha("Samantha", 200);
	sam.hires(samantha);
	Lord joe("Joe");
	PersonWithStrengthToFight randy("Randolf the Elder", 250);
	joe.battle(randy);
	joe.battle(sam);
	Lord janet("Janet");
	Swordsman hardy("TuckTuckTheHardy", 100);
	Swordsman stout("TuckTuckTheStout", 80);
	janet.hires(hardy);
	janet.hires(stout);
	hardy.state();
	PersonWithStrengthToFight barclay("Barclay the Bold", 300);
	janet.battle(barclay);
	janet.hires(samantha);
	Archer pethora("Pethora", 50);
	Archer thora("Thorapleth", 60);
	Wizard merlin("Merlin", 150);
	janet.hires(pethora);
	janet.hires(thora);
	sam.hires(merlin);
	janet.battle(barclay);
	sam.battle(barclay);
	joe.battle(barclay);
	system("pause");
}