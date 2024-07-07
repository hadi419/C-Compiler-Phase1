#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,                            // The name of the register to put the result in, destination register 
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");                 // regex for a number
    std::regex reId("^[a-z][a-z0-9]*$");            // regex for a variable/identifier
    
    if(regex_match(program->type, reNum)){          // handling a number
        std::cout<<"const "<<destReg<<" "<<program->type<<std::endl;
        
    }else if(regex_match(program->type, reId)){     // handling a variable/identifier
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<zero<<std::endl;

    }else if(program->type=="Param"){               // handling a parameter
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;

    }else if(program->type=="Seq"){                 // handling a sequence of statements or expressions
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }   
        
    }else if(program->type == "Input"){             // handling an input 
        std::cout << "input " << destReg << std::endl;

    }else if(program->type == "Output"){            // handling the code generation for an output
        std::string regA = makeName("regA");
        CompileRec(regA, program->branches[0]);
        std::string zero = makeName("zero");
        std::cout << "const " << zero << " 0" << std::endl;
        std::cout << "add " << destReg << " " << regA << " " << zero << std::endl;
        std::cout << "output " << regA << std::endl;

    }else if(program->type == "LessThan"){          // handling the code generation for LessThan
        std::string regA = makeName("regA");
        std::string regB = makeName("regB");
        CompileRec(regA, program->branches[0]);
        CompileRec(regB, program->branches[1]);
        std::cout << "lt " << destReg << " " << regA << " " << regB << std::endl;

    }else if(program->type == "Add"){               // addition
        std::string regA = makeName("regA");
        std::string regB = makeName("regB");
        CompileRec(regA, program->branches[0]);
        CompileRec(regB, program->branches[1]);
        std::cout << "add " << destReg << " " << regA << " " << regB << std::endl;

    }else if(program->type == "Sub"){               // sub
        std::string regA = makeName("regA");
        std::string regB = makeName("regB");
        CompileRec(regA, program->branches[0]);
        CompileRec(regB, program->branches[1]);
        std::cout << "sub " << destReg << " " << regA << " " << regB << std::endl;

    }else if(program->type == "Assign"){            // assignment
        CompileRec(destReg, program->branches[0]);
        std::string zero = makeName("zero");
        std::cout << "const " << zero << " 0" <<std::endl;
        std::cout << "add " << program->value << " " << destReg << " " << zero << std::endl;

    }else if(program->type == "If"){                // if else statement
        std::string regC = makeName("regC");
        CompileRec(regC, program->branches[0]);
        std::string zero = makeName("zero");
        std::cout << "const " << zero << " 0" << std::endl;
        std::string elseIfLbl = makeName("elseif");
        std::cout << "beq " << regC << " " << zero << " " << elseIfLbl << std::endl;
        CompileRec(destReg, program->branches[1]);
        std::string endIfLbl = makeName("endif");
        std::cout << "beq " << regC << " " << regC << " " << endIfLbl << std::endl;
        std::cout << ":" << elseIfLbl << std::endl;
        CompileRec(destReg, program->branches[2]);
        std::cout << ":" << endIfLbl << std::endl;

    }else if(program->type == "While"){             // while loop code generation
        std::string startWhileLbl = makeName("startWhileLbl");
        std::string endWhileLbl = makeName("endWhileLbl");
        std::string regC = makeName("regC");
        std::string zero = makeName("zero");
        std::cout << ":" << startWhileLbl << std::endl;
        CompileRec(regC, program->branches[0]);
        std::cout << "const " << zero << " 0" << std::endl;
        std::cout << "beq " << regC << " " << zero << " " << endWhileLbl << std::endl;
        CompileRec(destReg, program->branches[1]);
        std::cout << "beq " << regC << " " << regC << " " << startWhileLbl << std::endl;
        std::cout << ":" << endWhileLbl << std::endl;
        std::cout << "add " << destReg << " " << regC << " " << zero << std::endl;

    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");

    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");
    
    // Compile the whole thing
    CompileRec(res, program);
    
    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}
