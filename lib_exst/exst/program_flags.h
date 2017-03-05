#ifndef EXST_PROGRAM_PARAMETER_H
#define EXST_PROGRAM_PARAMETER_H

#include <exst/exst_types.h>
#include <string>

namespace exst {
    class ExstFlags {
    public:
        static ExstFlags &getInstance() {
            static ExstFlags param;
            return param;
        }

        ///
        unsigned long long widthCalcInterval = 1000;
        ///
        bool calculateTreeWidth = false;
        ///
        std::string iGraphPath = "";
        ///
        std::string rGraphPath = "";
        ///
        GraphFormat iGraphFormat = NONE;
        ///
        GraphFormat rGraphFormat = NONE;
        /// output path for the dependency graph
        std::string dGraphPath;
        /// output format for the dependency graph
        GraphFormat dGraphFormat = NONE;

        static bool parseParameter(ExstFlags *, const std::string &name, const std::string &value);
    };
}

#endif //EXST_PROGRAM_PARAMETER_H
