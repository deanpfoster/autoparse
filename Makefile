################################################################################
#
#
#                          SOURCE  CODE   (with lots of testing)
#
#
################################################################################

SHELL = /bin/bash
LOCALLDFLAGS = -L./lib
LOCALLDLIBS = -lutilities
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
current_target: test learn.main


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
only0: word.OK  history.OK forecast.OK feature.OK
################################################################################
word.test: word.o

history.test: history.o history.test.o

forecast.test:

feature.test:

################################################################################
#
#           L E V E L     O N E    T E S T I N G     C O D E
#
#          (This code can depend on level zero code)
#
only1:  dependency.OK transition_probability.OK forecast_constant.OK \
        value_of_forecasts.OK forecast_linear.OK eigenwords.OK tokenize.OK
#
################################################################################
dependency.test:   word.o

transition_probability.test: word.o

forecast_constant.test: forecast.o

forecast_linear.test: forecast.o

value_of_forecasts.test: history.o

eigenwords.test: word.o

tokenize.test: word.o


################################################################################
#
#           L E V E L     T W O    T E S T I N G     C O D E
#
#          (This code can depend on level zero or level one code)
#
only2:  lr.OK feature_generator.OK tp_eigenwords.OK train_forecast_linear.OK  \
        decorated_dependency.OK model.OK interval.OK
#
################################################################################
lr.test: dependency.o word.o

feature_generator.test: feature.o history.o dependency.o

tp_eigenwords.test: eigenwords.o word.o transition_probability.o

train_forecast_linear.test: forecast_linear.o forecast.o 

decorated_dependency.test: dependency.o word.o eigenwords.o

model.test: forecast.o history.o value_of_forecasts.o

interval.test: dependency.o word.o

################################################################################
#
#           L E V E L     T H R E E    T E S T I N G     C O D E
#
#          (This code can depend on level 0, 1, or 2 code)
#
only3: redo_parse.OK feature_eigenwords.OK feature_one_dimensional.OK \
       likelihood.OK similarity.OK
#
################################################################################

redo_parse.test: history.o dependency.o lr.o word.o

feature_one_dimensional.test: lr.o dependency.o word.o feature.o

feature_eigenwords.test: lr.o dependency.o word.o feature.o eigenwords.o

likelihood.test: dependency.o transition_probability.o word.o decorated_dependency.o eigenwords.o

similarity.test: dependency.o word.o interval.o

################################################################################
#           L E V E L     F O U R    T E S T I N G     C O D E
#          (This code can depend on level 0, 1, 2 or 3 code)
only4: statistical_parse.OK row.OK feature_interaction.OK maximum_likelihood.OK feature_shorten.OK
################################################################################

maximum_likelihood.test: dependency.o transition_probability.o likelihood.o word.o likelihood.o tp_eigenwords.o eigenwords.o decorated_dependency.o

statistical_parse.test: history.o dependency.o lr.o word.o redo_parse.o model.o value_of_forecasts.o forecast.o forecast_constant.o feature_generator.o feature.o

row.test: feature_generator.o lr.o word.o redo_parse.o dependency.o history.o feature.o

feature_interaction.test: feature.o feature_one_dimensional.o lr.o dependency.o word.o feature_eigenwords.o eigenwords.o

feature_shorten.test: feature.o lr.o dependency.o word.o feature_eigenwords.o eigenwords.o


################################################################################
#           L E V E L     F I V E    T E S T I N G     C O D E
#          (This code can depend on level 0, 1, 2, 3 or 4 code)
only5: contrast.OK
################################################################################
contrast.test: history.o dependency.o lr.o word.o redo_parse.o model.o \
               statistical_parse.o value_of_forecasts.o \
               forecast.o forecast_constant.o  transition_probability.o likelihood.o \
               feature_generator.o feature.o feature_one_dimensional.o\
               eigenwords.o tp_eigenwords.o row.o decorated_dependency.o


################################################################################
#           L E V E L     S I X    T E S T I N G     C O D E
only6: learn.OK
################################################################################


learn.test: history.o dependency.o lr.o word.o redo_parse.o model.o \
             statistical_parse.o value_of_forecasts.o \
             forecast.o forecast_constant.o  transition_probability.o likelihood.o \
             feature_generator.o feature.o feature_one_dimensional.o \
             contrast.o eigenwords.o tp_eigenwords.o feature_eigenwords.o maximum_likelihood.o \
             train_forecast_linear.o forecast_linear.o row.o value_of_forecasts.o tokenize.o decorated_dependency.o\
             feature_interaction.o feature_shorten.o


################################################################################
#           L E V E L     !!! I N F I N I T Y !!!    T E S T I N G     C O D E
#          (This code can depends on anything!)
onlyI:  
################################################################################

##########################################################################################################
#
#                       M A I N S
#
##########################################################################################################

learn.main: history.o dependency.o lr.o word.o redo_parse.o model.o  \
             statistical_parse.o value_of_forecasts.o \
             forecast.o forecast_constant.o  transition_probability.o likelihood.o \
             feature_generator.o feature.o feature_one_dimensional.o \
             contrast.o eigenwords.o tp_eigenwords.o feature_eigenwords.o maximum_likelihood.o \
             train_forecast_linear.o forecast_linear.o row.o value_of_forecasts.o tokenize.o learn.o \
             feature_interaction.o decorated_dependency.o feature_shorten.o

restore.main: history.o dependency.o lr.o word.o redo_parse.o model.o  \
             statistical_parse.o value_of_forecasts.o \
             forecast.o forecast_constant.o  transition_probability.o likelihood.o \
             feature_generator.o feature.o feature_one_dimensional.o \
             contrast.o eigenwords.o tp_eigenwords.o feature_eigenwords.o maximum_likelihood.o \
             train_forecast_linear.o forecast_linear.o row.o value_of_forecasts.o tokenize.o learn.o \
             feature_interaction.o decorated_dependency.o feature_shorten.o

learn.output: learn.main
	./learn.main | tee learn.output
	cat learn.output

learn.output.10k: learn.main eng_only.10k pretty.csv
	./$^ |tee $@

learn.output.all: learn.main eng_only pretty.csv
	./$^ |tee $@


##########################################################################################################
#
#  Makefile.rules has the following code I don't look at or edit very often.
#
##########################################################################################################

SOURCES = $(wildcard *.cc)

-include utilities/Makefile.rules

#  Put things here that aren't caught by a "make clean" that should be deleted

clean.local:


# flags: utilities/flags.sample
#	if [  -f ../flags ];  then touch ../flags; ln -s ../flags; else cp $< $@; fi

