'''
Created on Nov 8, 2012

@author: manju
'''

from misc.printer import Printer
from featureExtractor import FeatureExtractor
from time import time

from subprocess import Popen, PIPE
import signal
import threading
import json
import sys
from claspre2 import Claspre2


class Exst(Claspre2):
    '''
        Extract Feature from exst
    '''

    def __init__(self):
        '''
        Constructor
        '''
        Claspre2.__init__(self)

    def run_extractor(self, args_dic, instance):
        '''
            run extractor and look for instance features
            Parameter:
                args_dic : dictionary with options
                instance : instance to solve
        '''

        #TODO - total runtime
        '''
        start_time = time()
        '''
        ret = Claspre2.run_extractor(self, args_dic, instance)

        Printer.print_c("\nExtended Stats Feature Extraction:")
        self._set_args(args_dic, instance)

        cmd = self._cmd
        cmd[0] = cmd[0].replace("claspre", "exst")
        del cmd[1]
        del cmd[1]
        cmd.append("--stats=2")
        Printer.print_c(" ".join(cmd))

        signal.signal(signal.SIGINT, self.__clean_up_with_signal)
        signal.signal(signal.SIGHUP, self.__clean_up_with_signal)
        signal.signal(signal.SIGQUIT, self.__clean_up_with_signal)
        signal.signal(signal.SIGSEGV, self.__clean_up_with_signal)
        signal.signal(signal.SIGTERM, self.__clean_up_with_signal)
        signal.signal(signal.SIGXCPU, self.__clean_up_with_signal)
        signal.signal(signal.SIGXFSZ, self.__clean_up_with_signal)

        try:
            self._popen_ = Popen(cmd, stdin=PIPE, stdout=PIPE, stderr=PIPE)
            self._instance.seek(0)
            if isinstance(self._instance, file) and self._instance.name.endswith(".gz"):
                zcat_popen = Popen(["zcat", self._instance.name], stdout=PIPE)
                input_ = zcat_popen.stdout.read()
            else:
                input_ = self._instance.read()
            (out_, err_) = self._popen_.communicate(input=input_)
        except OSError:
            Printer.print_w("Feature extractor was unable to compute features (path correct?): %s" % (cmd))

        features = None
        try:
            # TODO - optional output
            '''
            end_time = time()
            time_taken = end_time - start_time
            out_ = out_.split("########## Extended Stats ##########")[1]
            pr = self._out[:len(self._out) - 3] + ",\n \"Time\" : " + str(time_taken) + "," + out_[3:]
            print pr
            '''
            feature_dict = json.loads(out_)
        except:
            try:
                Printer.print_w("Could not parse features. %s!" % (self._instance.name))
            except AttributeError:
                Printer.print_w("Could not parse features from stdin!")
            return features

        preprocessing_feats = feature_dict["Extended Stats"]

        dynamic_feats = []
        index = 1
        while True:
            restart_feats = feature_dict.get("Dynamic-%d" % (index))
            if restart_feats:
                dynamic_feats.extend(restart_feats)
            else:
                break
            index += 1

        opt_feats = feature_dict.get("Optimization")

        flat_feats = []
        flat_feats.extend([y for (x, y) in preprocessing_feats])
        flat_feats.extend([y for (x, y) in dynamic_feats])
        if opt_feats:
            flat_feats.extend([y for (x, y) in opt_feats])

        features = flat_feats
        ret += features

        return ret

    def __clean_up_with_signal(self, signal, frame):
        try:
            self._popen_.kill()
        except:
            pass
        sys.exit(1)
