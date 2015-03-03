// -*- c++ -*-


#include "shared_memory_stream.h"

// put other includes here
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Shared_memory_stream::~Shared_memory_stream()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Shared_memory_stream::Shared_memory_stream()
  :
  writer(1),  // start with writer having the lock
  reader(0),      //reader have to wait
  number_written(0),
  value()
{
  for(int i = 0; i < number_of_doubles; ++i)
    value[i] = 0;
};
////////////////////////////////////////////////////////////////////////////////////////////
//
//                                       W R I T E R
//
////////////////////////////////////////////////////////////////////////////////////////////

auto_parse::Shared_memory_writer::Shared_memory_writer(const std::string& name, int id)
  :
  mp_data(0)
{
  std::stringstream s;
  s << name << ":" << id;
  std::string full_name = s.str();
  //use old shared memory if exists else create a new one
  boost::interprocess::shared_memory_object shm(open_or_create, full_name, read_write);
  //set the size of the memory object
  shm.truncate(sizeof(Shared_memory_stream));
  //map the whole shared memory in this process
  boost::interprocess::mapped_region region(shm,read_write);
  void * addr = region.get_address();
  //create a shared memory buffer in memory
  mp_data = new (addr) shared_memory_buffer;
  mp_data->writer.wait(); // we have permission to write now
};

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

auto_parse::Shared_memory_writer&
auto_parse::Shared_memory_writer::operator<<(auto_parse::Shared_memory_writer & rhs, double d)
{
  mp_data.value[mp_data->write_pointer] = d;
  mp_data->reader.post();
  mp_data->write_pointer ++;
  assert(mp_data->write_pointer < mp_data->number_of_doubles);
  return *this; 
};

////////////////////////////////////////////////////////////////////////////////////////////
//
//                                       R E A D E R
//
////////////////////////////////////////////////////////////////////////////////////////////

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Shared_memory_reader::~Shared_memory_reader()
{
  // This is where we would delete the memory of we wanted to
  
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Shared_memory_reader::Shared_memory_reader(const std::string& name, int id)
  :
  mp_data(0),
  m_reader_pointer(0)
{
  std::stringstream s;
  s << name << ":" << id;
  std::string full_name = s.str();

  boost::interprocess::shared_memory_object shm(open_only, full_name, read_write);
  //Map the whole shared memory in this process
  boost::interprocess::mapped_region region(shm,read_write);
  void * addr = region.get_address();
  mp_data = static_cast(addr);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 

auto_parse::Shared_memory_reader&
auto_parse::Shared_memory_reader::operator>>(auto_parse::Shared_memory_reader & rhs, double& d)
{
  mp_data->reader.wait();
  assert(m_reader_pointer < mp_data->write_pointer);
  d = mp_data.value[m_reader_pointer];
  m_reader_pointer++;
  return *this; 
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


