/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#ifndef RICHARDSFLUX
#define RICHARDSFLUX

#include "Kernel.h"
#include "RichardsVarNames.h"

// Forward Declarations
class RichardsFlux;

template<>
InputParameters validParams<RichardsFlux>();

/**
 * Kernel = grad(permeability*relativepermeability/viscosity * (grad(pressure) - density*gravity))
 * This is mass flow according to the Richards equation
 */
class RichardsFlux : public Kernel
{
public:

  RichardsFlux(const std::string & name,
                        InputParameters parameters);


protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /**
   * holds info regarding the names of the Richards variables
   * and methods for extracting values of these variables
   */
  const RichardsVarNames & _richards_name_UO;

  /**
   * the index of this variable in the list of Richards variables
   * held by _richards_name_UO.  Eg
   * if richards_vars = 'pwater pgas poil' in the _richards_name_UO
   * and this kernel has variable = pgas, then _pvar = 1
   * This is used to index correctly into _viscosity, _seff, etc
   */
  unsigned int _pvar;

  /// Richards flux
  MaterialProperty<std::vector<RealVectorValue> > &_flux;

  /// d(Richards flux_i)/d(variable_j), here flux_i is the i_th flux, which is itself a RealVectorValue
  MaterialProperty<std::vector<std::vector<RealVectorValue> > > &_dflux_dv;

  /// d(Richards flux_i)/d(grad(variable_j)), here flux_i is the i_th flux, which is itself a RealVectorValue
  MaterialProperty<std::vector<std::vector<RealTensorValue> > > &_dflux_dgradv;

  /// d^2(Richards flux_i)/d(variable_j)/d(variable_k), here flux_i is the i_th flux, which is itself a RealVectorValue
  MaterialProperty<std::vector<std::vector<std::vector<RealVectorValue> > > > &_d2flux_dvdv;

  /// d^2(Richards flux_i)/d(grad(variable_j))/d(variable_k), here flux_i is the i_th flux, which is itself a RealVectorValue
  MaterialProperty<std::vector<std::vector<std::vector<RealTensorValue> > > > &_d2flux_dgradvdv;

  /// d^2(Richards flux_i)/d(variable_j)/d(grad(variable_k)), here flux_i is the i_th flux, which is itself a RealVectorValue
  MaterialProperty<std::vector<std::vector<std::vector<RealTensorValue> > > > &_d2flux_dvdgradv;



  /// grad_i grad_j porepressure.  This is used in SUPG
  VariableSecond & _second_u;

  /// interpolation function for the _second_u
  VariablePhiSecond & _second_phi;

  /// SUPGtau*SUPGvel (a vector of these if multiphase)
  MaterialProperty<std::vector<RealVectorValue> >&_tauvel_SUPG;

  /// derivative of SUPGtau*SUPGvel wrt grad(variable)
  MaterialProperty<std::vector<RealTensorValue> >&_dtauvel_SUPG_dgradv;

  /// derivative of SUPGtau*SUPGvel wrt variable
  MaterialProperty<std::vector<RealVectorValue> >&_dtauvel_SUPG_dv;
};

#endif //RICHARDSFLUX
