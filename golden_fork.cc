// -*- c++ -*-



#include "learn.h"
#include "assert.h"
#include "feature_shorten.h"
#include <iostream>
#include "feature_interaction.h"


#include "feature_interaction.h"
#include "feature_one_dimensional.h"
#include "feature_eigenwords.h"
#include "train_forecast_linear.h"
#include "row.h"
#include "contrast.h"
#include "tp_eigenwords.h"
#include "tp_iid.h"
#include "maximum_likelihood.h"
#include "redo_parse.h"
#include "gold_standard.h"
#include "golden_contrast.h"

#include "forecast_linear.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions



//////////////////////////////////////////////////////////////////////////////////////////////////
// I've been having trouble using threads: something crashes my memory.  I don't think it is
// me.  (But no programmer ever does.)  So I'm going to try switching the main loop to using
// fork instead of threads.
//
//     Master              slave
//       |
//       |
//       V
//    gets signal 0    starts life with !=0 signal
//       |                   |
//       |                   |
//       V                   V
//    does no work       does a fraction of work
//       |                   |
//       |                   |
//       |                   V
//       |             writes results on message board and dies
//       V
//    collects up messages and reconstitutes them into a new model
//
//////////////////////////////////////////////////////////////////////////////////////////////////



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model
auto_parse::golden_fork(const auto_parse::Statistical_parse& parser,
			const Feature_generator& feature_generator,
			double sampling_rate,
			std::vector<auto_parse::Gold_standard>::const_iterator gold_begin,
			std::vector<auto_parse::Words>::const_iterator begin,
			std::vector<auto_parse::Words>::const_iterator end)
{
  Model lr_model = parser.model();
  std::map<auto_parse::Action, auto_parse::Train_forecast_linear> all_training;
  for(auto_parse::Action a: parser.all_actions())
    training[a] = auto_parse::Train_forecast_linear(lr_model.forecast(a),sampling_rate);
  // note: "training" will be the aggregator for the master and the accumulator for the slaves
  int number_to_read = end - begin;

  // note: the const is silly--but it helps remind us that this is a read only object
  const auto_parse::Golden_contrast contrast(parser, feature_generator);

  message_queue queue = msgget("key???");

  ////////////////////////////////////////////////////////////////////////////////////
  //
  //  YIKES!  WE ARE BRAVELY CREATING THREADS!
  //
  ////////////////////////////////////////////////////////////////////////////////////

  int number_of_workers = 24;  // need to read this in eventually
  int worker_id = 0; // 0 = master
  for(int i = 0; i < number_of_workers; ++i) // only the main thread really does this look
    if(worker_id == 0) // i.e. we are the master
      {
	int who_are_we = fork();
	if(who_are_we != 0)
	  worker_id = i;
      };
  // We now have everything set up.  worker_id==0 for master, all others are slaves

  if(worker_id != 0)
    { // we are a SLAVE!
      int start = (worker_id * number_to_read) / number_of_workers;
      int end = ((worker_id + 1) * number_to_read) / number_of_workers;
      assert(start >= 0);
      assert(end <= number_to_read);

    for(int counter = start; counter < end; ++counter)
      {
	std::vector<auto_parse::Row> contrast_pair = contrast(*(gold_begin + counter) ,  *(begin + counter) );
	for(auto i = contrast_pair.begin(); i != contrast_pair.end(); ++i)
	    training[i->m_a](i->m_X, i->m_Y);
      };
    std::stringstream s;
    training.dump_to(s);
    msgsnd(training.str().c_str()); // I'm guessing it will need a cstring
    exit(0);  // done, so kill off this child
    }
  else
    {
      for(int i = 0; i < number_of_workers; ++i)
	{
	  msgrcv();
	  std::map<auto_parse::Action, auto_parse::Train_forecast_linear> child = read_message_queue(queue);
	  for(auto_parse::Action a: parser.all_actions())
	    training[a].merge(child[a]);
	}
      msgctl(IPC_RMID) // delete the queue
    }
  std::cout << "Sample size used for training: " << all_training[Action::shift].n() << std::endl;

  auto_parse::Model new_model; //  = parser.model();
  for(auto_parse::Action a : parser.all_actions())
    new_model.add_forecast(a,all_training[a].result());
  return new_model;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
