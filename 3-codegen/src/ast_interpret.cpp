#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, 
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");                         // regex for a number
    std::regex reId("^[a-z][a-z0-9]*$");                    // regex for a variable/identifier
    
    if( regex_match(program->type, reNum) ){                // number interpretation
        return std::atol(program->type.c_str());
        
    }else if(regex_match(program->type, reId)){             // variable/identifier interpretation
        return context.bindings.at(program->type.c_str());

    }else if(program->type=="Param"){                       // parameter interpretation
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
        
    }else if(program->type == "Input"){                     // input interpretation
        int32_t val;
        std::cin >> val;
        return val;

    }else if(program->type=="Output"){                      // output interpretation
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
        
    }else if(program->type == "LessThan"){                  // lessthan interpretation
        int32_t compA = Interpret(context, program->branches.at(0));
        int32_t compB = Interpret(context, program->branches.at(1));
        return compA < compB;

    }else if(program->type == "Add"){                       // addition interpretation
        int32_t addA = Interpret(context, program->branches.at(0));
        int32_t addB = Interpret(context, program->branches.at(1));
        return addA + addB;

    }else if(program->type == "Sub"){                       // sub interpretation 
        int32_t subA = Interpret(context, program->branches.at(0));
        int32_t subB = Interpret(context, program->branches.at(1));
        return subA - subB;

    }else if(program->type == "Assign"){                    // assignment interpretation
        int32_t val = Interpret(context, program->branches.at(0));
        std::map<std::string, int32_t>::iterator it = context.bindings.find(program->value.c_str());
        if(it != context.bindings.end()){
            unsigned i = atol(program->value.c_str());
            context.bindings.at(program->value.c_str()) = val;
        }else{
            context.bindings.insert(std::pair<std::string, int32_t>(program->value.c_str(),val));
        }
        return val;

    }else if(program->type == "Seq"){                       // sequence interpretation
        int32_t val;
        for(unsigned i = 0; i < program->branches.size(); i++){
            val = Interpret(context, program->branches.at(i));
        }
        return val;

    }else if(program->type == "If"){                        // if statement interpretation
        int32_t cond = Interpret(context, program->branches.at(0));
        if(cond == 0){
            int32_t val = Interpret(context, program->branches.at(2));
            return val;
        }else{
            int32_t val = Interpret(context, program->branches.at(1));
            return val;
        }

    }else if(program->type == "While"){                     // while loop interpretation
        int32_t cond = Interpret(context, program->branches.at(0));
        while(cond != 0){
            int32_t val = Interpret(context, program->branches.at(1));
            cond = Interpret(context, program->branches.at(0));
        }
        return 0;

    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
