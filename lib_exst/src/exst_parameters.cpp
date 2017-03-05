#include <exst/exst_parameters.h>

namespace exst {

    void ExtendedParameters::addRule(std::list<lit_type> body, std::list<lit_type> head) {

        std::list<exst::ParametersCalculator *>::iterator it;
        for (it = parameterCalc.begin(); it != parameterCalc.end(); it++) {
            (*it)->addRule(body, head);
        }

    }

    void ExtendedParameters::printParameters() {
        std::list<std::pair<std::string, std::string>> stats;
        std::list<exst::ParametersCalculator *>::iterator it;
        for (it = parameterCalc.begin(); it != parameterCalc.end(); it++) {
            stats.merge((*it)->getParameters());
        }

        std::cout << ",\"Extended Stats\" : [\n";
        std::list<std::pair<std::string, std::string>>::iterator it2;
        bool begin = true;
        for (it2 = stats.begin(); it2 != stats.end(); it2++) {
            if (begin) {
                std::cout << "  [\"" << it2->first << "\" , " << it2->second << "]\n";
                begin = false;
            } else {
                std::cout << "  ,[\"" << it2->first << "\" , " << it2->second << "]\n";
            }
        }
        std::cout << "]";

        std::list<exst::ParametersCalculator *>::iterator it3;
        for (it3 = parameterCalc.begin(); it3 != parameterCalc.end(); it3++) {
            std::list<std::string>::iterator it3_;
            std::list<std::string> addstats = (*it3)->getAdditionalParameters();
            for (it3_ = addstats.begin(); it3_ != addstats.end(); it3_++) {
                std::cout << "," << (*it3_);
            }
        }

        std::cout.flush();
    }

    void ExtendedParameters::updateAssignment(Clasp::LitVec new_assignment) {

        std::list<exst::ParametersCalculator *>::iterator it;
        for (it = parameterCalc.begin(); it != parameterCalc.end(); it++) {
            (*it)->updateAssignment(new_assignment);
        }

    }

    void ExtendedParameters::setSymbolTable(const Clasp::SymbolTable &symbolTable) {

        std::list<exst::ParametersCalculator *>::iterator it;
        for (it = parameterCalc.begin(); it != parameterCalc.end(); it++) {
            (*it)->setSymbolTable(symbolTable);
        }

    }

    void ExtendedParameters::addId(uint32_t before, uint32_t after) {

        std::list<exst::ParametersCalculator *>::iterator it;
        for (it = parameterCalc.begin(); it != parameterCalc.end(); it++) {
            (*it)->addId(before, after);
        }

    }

    void ExtendedParameters::addModel(const Clasp::Model &model) {

        std::list<exst::ParametersCalculator *>::iterator it;
        for (it = parameterCalc.begin(); it != parameterCalc.end(); it++) {
            (*it)->addModel(model);
        }

    }

}