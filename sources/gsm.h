//
// FILE: gsm.h -- definition of GSM (stack machine) type
//
// $Id$
//


#ifndef GSM_H
#define GSM_H


#include "gambitio.h"
#include "gtext.h"
#include "gsmincl.h"
#include "gsminstr.h"
#include "gsmhash.h"

class FuncDescObj;
class CallFuncObj;
class Portion;
class RefHashTable;
class FunctionHashTable;
class FuncInfoType;

class gText;
class gRational;

class Node;
class Infoset;
class Efg;

template <class T> class gList;
template <class T> class gStack;

template <class T> class RefCountHashTable;


#define GCL_VERSION   0.95

class GSM
{
friend class gclFunctionCall;
friend class gclVarName;
private:
  static int _NumObj;

  static RefCountHashTable< void* > _GameRefCount;

  bool _Verbose;

  gInput&  _StdIn;
  gOutput& _StdOut;
  gOutput& _StdErr;

  Portion* _OUTPUT;
  Portion* _INPUT;
  Portion* _NULL;


  gStack< gStack< Portion* >* >* _StackStack;
  gStack< RefHashTable* >*       _RefTableStack;
  gStack< gText >*             _FuncNameStack;
  RefHashTable                   _GlobalRefTable;
  FunctionHashTable*             _FuncTable;

  void _BindCheck ( void ) const;
  bool _Bind ( const gText& param_name ) const;

  Portion* _VarRemove    ( const gText& var_name );

  static void _ErrorMessage
    (
     gOutput&        s,
     const int       error_num = 0,
     const long& num1      = 0, 
     const long& num2      = 0,
     const gText&  str1      = "",
     const gText&  str2      = "",
     const gText&  str3      = ""
     );

  // This function is located in gsmfunc.cc
  void InitFunctions( void );


public:
  GSM( int size = 10, 
      gInput&  s_in  = gin, 
      gOutput& s_out = gout, 
      gOutput& s_err = gerr );
  ~GSM();

  static int& GameRefCount(void*);

  bool PushRef  ( const gText& ref );
  void _ResolveRef ( Portion*& p );


  // Assign() will delete lhs and rhs
  Portion* Assign       ( Portion* lhs, Portion* rhs );
  bool     VarDefine    ( const gText& var_name, Portion* p );
  bool     VarIsDefined ( const gText& var_name ) const;
  Portion* VarValue     ( const gText& var_name ) const;
  bool     UnAssign     ( Portion * );
  Portion* UnAssignExt  ( Portion * );

  bool AddFunction( FuncDescObj* func );
  bool DeleteFunction( FuncDescObj* func );

  Portion* Execute(gclExpression *, bool user_func = false );
  Portion* ExecuteUserFunc( gclExpression& program, 
			   const FuncInfoType& func_info,
			   Portion** param, 
			   const gText& funcname );
  gText UserFuncName( void ) const;
  
  void Clear  ( void );

  Portion* PopValue( void );

  Portion* Help(gText text, bool udf, bool bif, bool getdesc = false );
  Portion* HelpVars(gText text);

  void InvalidateGameProfile( void* game, bool IsEfg );
  void UnAssignGameElement( void* game, bool IsEfg, PortionSpec spec );  

  // Unassigns a SINGLE given element of an Efg
  void UnAssignEfgElement( Efg* game, PortionSpec spec, void* data );

  // This function will unassign an infoset and all associated actions
  void UnAssignEfgInfoset( Efg* game, Infoset* infoset );

  // This function will unassign the subtree rooted by the given node
  void UnAssignEfgSubTree( Efg* game, Node* node ); 






  void GlobalVarDefine     ( const gText& var_name, Portion* p );
  bool GlobalVarIsDefined  ( const gText& var_name ) const;
  Portion* GlobalVarValue  ( const gText& var_name ) const;
  void GlobalVarRemove     ( const gText& var_name );

};



#endif  // GSM_H

