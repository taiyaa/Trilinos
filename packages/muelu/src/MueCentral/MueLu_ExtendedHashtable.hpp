/*
 * ExtendedHashtable.hpp
 *
 *  Created on: Aug 29, 2011
 *      Author: wiesner
 */
#ifndef MUELU_EXTENDENDEDHASHTABLE_HPP
#define MUELU_EXTENDENDEDHASHTABLE_HPP

#include <MueLu_Exceptions.hpp>
#include <MueLu_BaseClass.hpp>
#include <MueLu_FactoryBase.hpp>
#include <Teuchos_TabularOutputter.hpp>

#undef RCPINTERFACE

namespace MueLu
{
  using std::string;

  namespace UTILS
  {
    class ExtendedHashtable : MueLu::BaseClass
    {
      public:
      inline ExtendedHashtable() {};

#ifdef RCPINTERFACE
      template<typename Value> inline void Set(const string& ename, const Value& evalue, const RCP<const FactoryBase>& factory)
      {
        Set(ename,evalue,factory.get());
      }
#endif

      template<typename Value> inline void Set(const string& ename, const Value& evalue, const FactoryBase* factory)
      {
        // if ename does not exist at all
        if (!dataTable_.count(ename) > 0)
        {
          std::map<const MueLu::FactoryBase*,Teuchos::any> newmapData;
          dataTable_[ename] = newmapData; // empty map
        }

        std::map<const MueLu::FactoryBase*,Teuchos::any>& mapData = dataTable_[ename];
        mapData[factory] = evalue;
      }

#ifdef RCPINTERFACE
      template<typename Value> Value& Get(const string& ename, const RCP<const FactoryBase>& factory)
      {
        return Get<Value>(ename,factory.get());
      }
#endif

      template<typename Value> Value& Get(const string& ename, const FactoryBase* factory)
      {
        if(!dataTable_.count(ename) > 0)
        {
          std::stringstream str; str << "key" << ename << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }

        if(!dataTable_[ename].count(factory) > 0)
        {
          std::stringstream str; str << "key " << ename << " generated by " << factory << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }

        std::map<const MueLu::FactoryBase*,Teuchos::any> mapData = dataTable_[ename];
        Value retData = Teuchos::any_cast<Value>( mapData[factory]);
        Value& refData = retData;
        return refData;
      }

#ifdef RCPINTERFACE
      template<typename Value> void Get(const string& ename, Value& value, RCP<const FactoryBase> factory)
      {
        Get(ename,value,factory.get());
      }
#endif

      // todo: remove me??
      template<typename Value> void Get(const string& ename, Value& value, const FactoryBase* factory)
      {
        if(!dataTable_.count(ename) > 0)
        {
          std::stringstream str; str << "key" << ename << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }

        if(!dataTable_[ename].count(factory) > 0)
        {
          std::stringstream str; str << "key " << ename << " generated by " << factory << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }

        std::map<const MueLu::FactoryBase*,Teuchos::any> mapData = dataTable_[ename];
        value = Teuchos::any_cast<Value>( mapData[factory]);
      }

#ifdef RCPINTERFACE
      void Remove(const string& ename, const RCP<const FactoryBase>& factory)
      {
        if(!dataTable_.count(ename) > 0)
        {
          std::stringstream str; str << "key" << ename << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }

        Remove(ename,factory.get());
      }
#endif

      void Remove(const string& ename, const FactoryBase* factory)
      {
        if(!dataTable_.count(ename) > 0)
        {
          std::stringstream str; str << "key" << ename << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }

        if(!dataTable_[ename].count(factory) > 0)
        {
          std::stringstream str; str << "key " << ename << " generated by " << factory << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }

        if(dataTable_[ename].erase(factory)!=1)
        {
          std::stringstream str; str << "error: could not erase " << ename << "generated gy " << factory;
          throw(Exceptions::RuntimeError(str.str()));
        }

        // check if there exist other instances of 'ename' (generated by other factories than 'factory')
        if(dataTable_.count(ename) == 0)
          dataTable_.erase(ename); // last instance of 'ename' can be removed
      }

#ifdef RCPINTERFACE
      inline std::string GetType(const string& ename, const RCP<const FactoryBase>& factory)
      {
        return GetType(ename,factory.get());
      }
#endif

      inline std::string GetType(const string& ename, const FactoryBase* factory)
      {
        if(!dataTable_.count(ename) > 0)
        {
          std::stringstream str; str << "key" << ename << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }

        if(!dataTable_[ename].count(factory) > 0)
        {
          std::stringstream str; str << "key " << ename << " generated by " << factory << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }

        std::map<const MueLu::FactoryBase*,Teuchos::any> mapData = dataTable_[ename];
        return mapData[factory].typeName();
      }

#ifdef RCPINTERFACE
      bool isKey(const string& ename, const RCP<const FactoryBase>& factory)
      {
        // check if ename exists
        if (!dataTable_.count(ename) > 0) return false;

        return isKey(ename,factory.get());
      }
#endif

      bool isKey(const string& ename, const FactoryBase* factory)
      {
        // check if ename exists
        if (!dataTable_.count(ename) > 0) return false;

        // resolve factory to a valid factory ptr
        if (dataTable_[ename].count(factory) > 0) return true;

        return false;
      }

      std::vector<string> keys()
      {
        std::vector<string> v;
        for(std::map<string,std::map<const MueLu::FactoryBase*,Teuchos::any> >::iterator it = dataTable_.begin(); it!=dataTable_.end(); ++it)
        {
          v.push_back(it->first);
        }
        return v;
      }

      std::vector<const MueLu::FactoryBase*> handles(const string& ename)
      {
        if(!dataTable_.count(ename) > 0)
        {
          std::stringstream str; str << "key" << ename << " does not exist in Hashtable.";
          throw(Exceptions::RuntimeError(str.str()));
        }


        std::vector<const MueLu::FactoryBase*> v;
        std::map<const MueLu::FactoryBase*,Teuchos::any> mapData = dataTable_[ename];
        for(std::map<const MueLu::FactoryBase*,Teuchos::any>::iterator it = mapData.begin(); it!=mapData.end(); ++it)
        {
          v.push_back(it->first);
        }
        return v;
      }

      void Print(std::ostream &out)
      {
        Teuchos::TabularOutputter outputter(out);
        outputter.pushFieldSpec("name", Teuchos::TabularOutputter::STRING,Teuchos::TabularOutputter::LEFT,Teuchos::TabularOutputter::GENERAL,12);
        outputter.pushFieldSpec("gen. factory addr.", Teuchos::TabularOutputter::STRING,Teuchos::TabularOutputter::LEFT, Teuchos::TabularOutputter::GENERAL, 18);
        outputter.pushFieldSpec("type", Teuchos::TabularOutputter::STRING,Teuchos::TabularOutputter::LEFT, Teuchos::TabularOutputter::GENERAL, 18);
        outputter.outputHeader();

        std::vector<std::string> ekeys = keys();
        for (std::vector<std::string>::iterator it = ekeys.begin(); it != ekeys.end(); it++)
        {
          std::vector<const MueLu::FactoryBase*> ehandles = handles(*it);
          for (std::vector<const MueLu::FactoryBase*>::iterator kt = ehandles.begin(); kt != ehandles.end(); kt++)
          {
            outputter.outputField(*it);
            outputter.outputField(*kt);
            outputter.outputField(GetType(*it,*kt));
            outputter.nextRow();
          }
        }
      }

      //! Return a simple one-line description of this object.
      std::string description() const
      {
        return "ExtendedHashtable";
      }

      //! Print the object with some verbosity level to an FancyOStream object.
      void describe(Teuchos::FancyOStream &out, const Teuchos::EVerbosityLevel verbLevel=Teuchos::Describable::verbLevel_default) const
      {
        if (verbLevel != Teuchos::VERB_NONE)
        {
          out << description() << std::endl;
        }
      }

      private:
      /*const MueLu::FactoryBase* resolveFactoryPtr(const string& ename, const RCP<const FactoryBase>& fact)
      {
        const FactoryBase* ptrFactory = fact.get(); // this is the memory ptr to the Factory, we are searching for

        // ptrFactory = NULL: no factory at all
        if(ptrFactory==NULL)
          return NULL;

        std::vector<const MueLu::FactoryBase*> ehandles = handles(ename);
        for (std::vector<const MueLu::FactoryBase*>::iterator kt = ehandles.begin(); kt != ehandles.end(); kt++)
        {
          if(*kt==ptrFactory)
          {
            if((*kt)->getID() == fact->getID())
              return *kt;
            else
              throw(Exceptions::RuntimeError("Ooops. Two factories have the same memory address but different ids?"));
          }
        }

        // no corresponding already existing Teuchos::RCP<FactoryBase> found
        // return memory ptr of fact
        return fact.get();
      }*/


      private:
      std::map<const string,std::map<const MueLu::FactoryBase*,Teuchos::any> > dataTable_;

    };

  }
}


#endif /* MUELU_EXTENDENDEDHASHTABLE_HPP */
