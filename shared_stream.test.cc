//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "shared_stream.h"

namespace auto_parse
{
  void test_shared_stream()
  {
    std::cout << "\n\n\n\t\t\t SHARED_STREAM  SHARED_STREAM  SHARED_STREAM\n\n\n"<< std::endl;
    {
      ////////////////////////////////////////////////////////////////////////////////////
      //
      //   YIKES!  WE ARE BRAVELY CREATING FORKS!
      //
      //   This is only testable if we do a FORK!  So it will be much more interesting testing code than usual
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
	  Shared_memory_writer out("testing",worker_id);
	  for(int i = 0; i < 5; ++i)
	    out << 100 * worker_id + i;
	};
      exit(0);  // done, so kill off this child
    }
  else
    {
      for(int i = 0; i < number_of_workers; ++i)
	{
	  Shared_memory_reader in("testing",i);
	  double d;
	  for(int i = 0; i < 5; ++i)
	    {
	      in >> d;
	      std::out << d << " ";
	    };
	  std::out << std::endl;
	}
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_shared_stream();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif

