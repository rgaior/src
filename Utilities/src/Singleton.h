#ifndef _utl61_Singleton_h_
#define _utl61_Singleton_h_


namespace utl {

  /**
   * \class Singleton Singleton.h utl61/Singleton.h
   *
   * \brief Curiously Recurring Template Pattern (CRTP) for Meyers singleton
   *
   * The singleton class is implemented as follows
   * \code
   * #include <utl61/Singleton.h>
   *
   * class SomeClass : public utl61::Singleton<SomeClass> {
   *   ...
   * private:
   *   // prevent creation, destruction
   *   SomeClass() { }
   *   ~SomeClass() { }
   *
   *   friend class utl61::Singleton<SomeClass>;
   * };
   * \endcode
   * Singleton automatically prevents copying of the derived class.
   *
   * \author Darko Veberic
   * \date 9 Aug 2006
   * \version $Id: Singleton.h 86 2010-09-08 19:19:05Z munger $
   * \ingroup stl
   */

  template<typename T>
  class Singleton {
  public:
    static T& GetInstance()
    { static T instance; return instance; }

  protected:
    // derived class can call ctor and dtor
    Singleton() { }
    ~Singleton() { }

  private:
    // no one should do copies
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

  };


}


#endif
