#ifndef EXST_FLAGS_H
#define EXST_FLAGS_H

#include <exst/exst_types.h>
#include <string>

namespace exst {
    class ExstFlags {
    public:
        static ExstFlags &getInstance() {
            static ExstFlags param;
            return param;
        }

        /// number of assignments between each reduct generation
        unsigned long long widthCalcInterval = 1000;
        /// flag to calculate the treewidth
        bool calculateTreeWidth = false;
        /// output path for the incidence graph
        std::string iGraphPath = "";
        /// output path for the reduct graph
        std::string rGraphPath = "";
        /// output format for the incidence graph
        GraphFormat iGraphFormat = NONE;
        /// output format for the reduct graph
        GraphFormat rGraphFormat = NONE;
        /// output path for the dependency graph
        std::string dGraphPath;
        /// output format for the dependency graph
        GraphFormat dGraphFormat = NONE;

        /**
         * function to parse the flags which belong to the extension
         * @param name
         *      the name of the flag
         * @param value
         *      the value of the flag
         * @return
         *      true - if parsing was successfull
         *      false - otherwise
         */
        static bool parseFlags(ExstFlags *, const std::string &name, const std::string &value);
    };
}

#endif //EXST_FLAGS_H
