// -*- c++ -*-

#ifndef INCLUDED_SHARED_MEMORY_STREAM
#define INCLUDED_SHARED_MEMORY_STREAM

#include < boost/interprocess/sync/interprocess_semaphore.hpp >



namespace auto_parse
{
  class Shared_memory_reader;
  class Shared_memory_writer;

  class Shared_memory_stream
  {
    ~Shared_memory_stream();
  private: // Only friends can create this buffer
    Shared_memory_buffer();
  private:
    friend class Shared_memory_reader;
    friend class Shared_memory_writer;
    boost::interprocess::interprocess_semaphore writer, reader;
    const int number_of_doubles = 100;
    int write_pointer;
    double value[number_of_doubles];
  };


  class Shared_memory_writer
  {
  public:
    Shared_memory_writer(const std::string& name, int id);
    Shared_memory_stream& operator<<(Shared_memory_stream&, double);
    
  private:
    Shared_memory_stream* mp_data;
  }
  
  class Shared_memory_reader
  {
  public:
    Shared_memory_reader(const std::string& name, int id);
    Shared_memory_stream& operator>>(Shared_memory_stream&, double&);
    bool eof() const;
    void reset(); // allows writer to write again
    
  private:
    Shared_memory_stream* mp_data;
    int m_reader_pointer;
  }

}
