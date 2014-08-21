################################################################################
#
#
#                          SOURCE  CODE   (with lots of testing)
#
#
################################################################################

SHELL = /bin/bash
include flags

################################################################################
#
#                                    TARGETS
#
#   install     installs library
#   test        test all classes in a levelized fashion
#
#   test0       tests level zero code
#   test1       tests level one  code
#   test[2..5]  ...
#
#   clean       cleans .o files and executables
#   very_clean  kills everything we can think of.  Back to the beginning.
#
################################################################################

current_target: test


################################################################################
#
#                      L E V E L   T E S T I N G     C O D E
#
#  This weakly encourages levelization.  Each level is a target called only0, only1,
#  only2, etc.  These are then glued together into test0, test1, test2, etc.  (see Makefile.rules
# 
################################################################################


################################################################################
#
#           L E V E L       Z E R O     T E S T I N G     C O D E
#
#          (This code should not depend on any other code)
#
only0: word.OK  history.OK forecast.OK
################################################################################
word.test: word.o

history.test: history.o history.test.o

forecast.test:

################################################################################
#
#           L E V E L     O N E    T E S T I N G     C O D E
#
#          (This code can depend on level zero code)
#
only1:  dependency.OK transition_probability.OK forecast_constant.OK model.OK value_of_forecasts.OK
#
################################################################################
dependency.test: dependency.o  word.o

transition_probability.test: word.o transition_probability.o

forecast_constant.test: forecast.o

model.test: forecast.o history.o

value_of_forecasts.test: history.o

################################################################################
#
#           L E V E L     T W O    T E S T I N G     C O D E
#
#          (This code can depend on level zero or level one code)
#
only2:  lr.OK likelihood.OK
#
################################################################################
lr.test: dependency.o lr.o word.o

likelihood.test: dependency.o transition_probability.o likelihood.o word.o

################################################################################
#
#           L E V E L     T H R E E    T E S T I N G     C O D E
#
#          (This code can depend on level 0, 1, or 2 code)
#
only3: redo_parse.OK
#
################################################################################

redo_parse.test: history.o dependency.o lr.o word.o redo_parse.o

################################################################################
#           L E V E L     F O U R    T E S T I N G     C O D E
#          (This code can depend on level 0, 1, 2 or 3 code)
only4: 
################################################################################

################################################################################
#           L E V E L     F I V E    T E S T I N G     C O D E
#          (This code can depend on level 0, 1, 2, 3 or 4 code)
only5: 
################################################################################

################################################################################
#           L E V E L     S I X    T E S T I N G     C O D E
only6: 
################################################################################

################################################################################
#           L E V E L     !!! I N F I N I T Y !!!    T E S T I N G     C O D E
#          (This code can depends on anything!)
onlyI:
################################################################################


##########################################################################################################
#
#  Makefile.rules has the following code I don't look at or edit very often.
#
##########################################################################################################

SOURCES = dependency.cc


-include utilities/Makefile.rules

#  Put things here that aren't caught by a "make clean" that should be deleted

clean.local:


# flags: utilities/flags.sample
#	if [  -f ../flags ];  then touch ../flags; ln -s ../flags; else cp $< $@; fi

