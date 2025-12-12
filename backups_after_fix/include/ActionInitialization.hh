#ifndef ACTIONINITIALIZATION_H
#define ACTIONINITIALIZATION_H

#include "G4VUserActionInitialization.hh"

namespace B3 {

class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization();
  ~ActionInitialization() override;

  void BuildForMaster() const override;
  void Build() const override;
};

} // namespace B3

#endif // ACTIONINITIALIZATION_H
