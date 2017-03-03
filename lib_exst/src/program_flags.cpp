#include <exst/program_flags.h>

namespace exst {
    bool exst::ProgramParameter::parseParameter(ProgramParameter *, const std::string &name,
                                                const std::string &value) {
        exst::GraphFormat format;
        bool file = false;
        std::string path;

        if (value.length() == 0) {
            return false;
        }
        try {
            switch (std::stoi(value.substr(0, 1), nullptr)) {
                case 0 :
                    format = DIMACS;
                    break;
                case 1 :
                    format = GR;
                    break;
                case 2 :
                    format = GML;
                    break;
                default:
                    format = NONE;
            }
            if (value.length() > 1) {
                path = value.substr(2);
                file = true;
            }
        } catch (int e) {
            return false;
        }

        if (name.compare("printDgraph") == 0) {
            ProgramParameter::getInstance().dGraphFormat = format;
            if (file) {
                ProgramParameter::getInstance().dGraphPath = path;
            }
        } else if (name.compare("printIgraph") == 0) {
            ProgramParameter::getInstance().iGraphFormat = format;
            if (file) {
                ProgramParameter::getInstance().iGraphPath = path;
            }
        } else if (name.compare("printRgraph") == 0) {
            ProgramParameter::getInstance().rGraphFormat = format;
            if (file) {
                ProgramParameter::getInstance().rGraphPath = path;
            }
        } else {
            return false;
        }
        return true;
    }
}