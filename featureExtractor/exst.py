from misc.printer import Printer
from featureExtractor import FeatureExtractor

from subprocess import Popen, PIPE
import signal
import json
import sys
from claspre2 import Claspre2


class Exst(Claspre2):
    '''
        Extract Feature from exst
    '''

    def __init__(self):
        Claspre2.__init__(self)

    def run_extractor(self, args_dic, instance):
        '''
            run extractor and look for instance features
            Parameter:
                args_dic : dictionary with options
                instance : instance to solve
        '''

        # run claspre extractor
        ret = Claspre2.run_extractor(self, args_dic, instance)

        Printer.print_c("\nExtended Stats Feature Extraction:")
        self._set_args(args_dic, instance)

        cmd = self._cmd
        cmd[0] = cmd[0].replace("claspre", "exst")
        del cmd[1]
        del cmd[1]
        cmd.append("--outf=2")
        cmd.append("--stats=2")
        cmd.append("--time-limit=120")
        Printer.print_c(" ".join(cmd))

        signal.signal(signal.SIGINT, self.__clean_up_with_signal)
        signal.signal(signal.SIGHUP, self.__clean_up_with_signal)
        signal.signal(signal.SIGQUIT, self.__clean_up_with_signal)
        signal.signal(signal.SIGSEGV, self.__clean_up_with_signal)
        signal.signal(signal.SIGTERM, self.__clean_up_with_signal)
        signal.signal(signal.SIGXCPU, self.__clean_up_with_signal)
        signal.signal(signal.SIGXFSZ, self.__clean_up_with_signal)

        try:
            # start exst
            self._popen_ = Popen(cmd, stdin=PIPE, stdout=PIPE, stderr=PIPE)
            self._instance.seek(0)
            if isinstance(self._instance, file) and self._instance.name.endswith(".gz"):
                zcat_popen = Popen(["zcat", self._instance.name], stdout=PIPE)
                input_ = zcat_popen.stdout.read()
            else:
                input_ = self._instance.read()
            # get exst output
            (out_, err_) = self._popen_.communicate(input=input_)
        except OSError:
            Printer.print_w("Feature extractor was unable to compute features (path correct?): %s" % (cmd))

        try:
            # extract features from output
            feature_dict = json.loads(out_)
        except:
            try:
                Printer.print_w("Could not parse features. %s!" % (self._instance.name))
            except AttributeError:
                Printer.print_w("Could not parse features from stdin!")
            return ret

        # get Extended Stats element from json
        preprocessing_feats = feature_dict["Stats"]["Extended Stats"]

        flat_feats = []
        flat_feats.extend([y for (x, y) in preprocessing_feats])

        # combine claspre and extended features
        ret += flat_feats

        return ret

    def __clean_up_with_signal(self, signal, frame):
        try:
            self._popen_.kill()
        except:
            pass
        sys.exit(1)
