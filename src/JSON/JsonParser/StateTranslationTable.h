//-*-C++-*-
// Author: Michael S. Summers (ORNL)
/** \ingroup JsonParser */
/*@{*/

/*! \file StateTranslationTable.h  
 *
 *  
 *
 */

#ifndef  JsonParser_StateTranslationTable_H
#define  JsonParser_StateTranslationTable_H

#include "StatesAndActionsMixin.h"

namespace JsonParser {

  class StateTranslationTable: 
    public CharacterMapper, 
    public StatesAndActionsMixin
  {
  public:

    // returns a StateType (>0) or an ActionType (<0)
    StatesAndActionsType operator() (StatesAndActionsType currentState, CharacterClass cls) {

      if (!(currentState < NR_STATES) || !(cls < NR_CLASSES) ) {
	std::ostringstream msg;
	msg << "StateTranslationTable ("<< currentState << " < " << NR_STATES << "," << cls << " < " << NR_CLASSES << ")\n";
	msg << "StateTranslationTable ("<< name(currentState) << "," << clsName(cls) << ")\n";
	throw std::logic_error(msg.str());
      }
      
      static StatesAndActionsType state_transition_table[NR_STATES][NR_CLASSES] = {
	
	/* 
	  The state transition table takes the current state and the current symbol,
	  and returns either a new state or an action. An action is represented as a
	  negative number. A JSON text is accepted if at the end of the text the
	  state is OK and if the mode is MODE_DONE.

	  white                                      1-9                                   ABCDF  etc
	  space |  {  }  [  ]  :  ,  "  \  /  +  -  .  0  |  a  b  c  d  e  f  l  n  r  s  t  u  |  E  |  * */
	/*start  GO*/ {GO,GO,BO,__,BA,__,__,__,__,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*ok     OK*/ {OK,OK,__,EE,__,EA,__,CM,__,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*object OB*/ {OB,OB,__,EO,__,__,__,CM,SB,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*key    KE*/ {KE,KE,__,__,__,__,__,__,SB,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*colon  CO*/ {CO,CO,__,__,__,__,CN,__,__,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*value  VA*/ {VA,VA,BO,__,BA,__,__,__,SB,__,CB,__,MX,__,ZX,IX,__,__,__,__,__,FA,__,NU,__,__,TR,__,__,__,__,__},
	/*array  AR*/ {AR,AR,BO,__,BA,EA,__,__,SB,__,CB,__,MX,__,ZX,IX,__,__,__,__,__,FA,__,NU,__,__,TR,__,__,__,__,__},
	/*string ST*/ {ST,__,ST,ST,ST,ST,ST,ST,SE,EX,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST},
	/*escape ES*/ {__,__,__,__,__,__,__,__,ST,ST,ST,__,__,__,__,__,__,ST,__,__,__,ST,__,ST,ST,__,ST,U1,__,__,__,__},
	/*u1     U1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,U2,U2,U2,U2,U2,U2,U2,U2,__,__,__,__,__,__,U2,U2,__,__},
	/*u2     U2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,U3,U3,U3,U3,U3,U3,U3,U3,__,__,__,__,__,__,U3,U3,__,__},
	/*u3     U3*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,U4,U4,U4,U4,U4,U4,U4,U4,__,__,__,__,__,__,U4,U4,__,__},
	/*u4     U4*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,UC,UC,UC,UC,UC,UC,UC,UC,__,__,__,__,__,__,UC,UC,__,__},
	/*minus  MI*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,ZE,IT,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*zero   ZE*/ {OK,OK,__,EO,__,EA,__,CM,__,__,CB,__,__,DF,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*int    IT*/ {OK,OK,__,EO,__,EA,__,CM,__,__,CB,__,__,DF,IT,IT,__,__,__,__,DE,__,__,__,__,__,__,__,__,DE,__,__},
	/*frac   FR*/ {OK,OK,__,EO,__,EA,__,CM,__,__,CB,__,__,__,FR,FR,__,__,__,__,E1,__,__,__,__,__,__,__,__,E1,__,__},
	/*e      E1*/ {__,__,__,__,__,__,__,__,__,__,__,E2,E2,__,E3,E3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*ex     E2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,E3,E3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*exp    E3*/ {OK,OK,__,EO,__,EA,__,CM,__,__,__,__,__,__,E3,E3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*tr     T1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,T2,__,__,__,__,__,__,__},
	/*tru    T2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,T3,__,__,__,__},
	/*true   T3*/ {__,__,__,__,__,__,__,__,__,__,CB,__,__,__,__,__,__,__,__,__,OK,__,__,__,__,__,__,__,__,__,__,__},
	/*fa     F1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,F2,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*fal    F2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,F3,__,__,__,__,__,__,__,__,__},
	/*fals   F3*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,F4,__,__,__,__,__,__},
	/*false  F4*/ {__,__,__,__,__,__,__,__,__,__,CB,__,__,__,__,__,__,__,__,__,OK,__,__,__,__,__,__,__,__,__,__,__},
	/*nu     N1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,N2,__,__,__,__},
	/*nul    N2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,N3,__,__,__,__,__,__,__,__,__},
	/*null   N3*/ {__,__,__,__,__,__,__,__,__,__,CB,__,__,__,__,__,__,__,__,__,__,__,OK,__,__,__,__,__,__,__,__,__},
	/*/      C1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,C2},
	/*/*     C2*/ {C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C3},
	/**      C3*/ {C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,CE,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C3},
	/*_.     FX*/ {OK,OK,__,EO,__,EA,__,CM,__,__,__,__,__,__,FR,FR,__,__,__,__,E1,__,__,__,__,__,__,__,__,E1,__,__},
	/*\      D1*/ {__,__,__,__,__,__,__,__,__,D2,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
	/*\      D2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,U1,__,__,__,__},
      };

      StatesAndActionsType result(state_transition_table[currentState][cls]);

      std::cout << "\nStateTransitionTable.nextStateOrAction(state='" << name(currentState) << "',cls='" << clsName(cls) << "')  = (" << name(result) << ") \n";
      
      return result;
    }
  };

} // end namespace JsonParser


/*@}*/
#endif
