#ifndef BETAG4Application_HH
#define BETAG4Application_HH
#include <TVirtualMCApplication.h>

class BETAG4Application : public TVirtualMCApplication {
  public:
    BETAG4Application(const char *name, const char *title);
    BETAG4Application();
    virtual ~BETAG4Application();
  
    // static access method
    static BETAG4Application* Instance(); 

    // methods
    void InitMC(const char *setup);
    void RunMC(Int_t nofEvents);
    void FinishRun();
 
    virtual void ConstructGeometry();
    virtual void InitGeometry();
    virtual void GeneratePrimaries();
    virtual void BeginEvent();
    virtual void BeginPrimary();
    virtual void PreTrack();
    virtual void Stepping();
    virtual void PostTrack();
    virtual void FinishPrimary();
    virtual void FinishEvent();

    // methods for tests
    void SetOldGeometry(Bool_t oldGeometry = kTRUE);
    void TestVMCGeometryGetters();

  private:
    // methods
    void ConstructMaterials();
    void ConstructVolumes();

//   BETAG4Application();
//   BETAG4Application(int argc,char** argv);
//   ~BETAG4Application();

    // data members
    TVirtualMCStack*  fStack;       ///< The VMC stack
    TVirtualMagField* fMagField;    ///< The magnetic field 
    Int_t             fImedAr;      ///< The Argon gas medium Id
    Int_t             fImedAl;      ///< The Aluminium medium Id
    Int_t             fImedPb;      ///< The Lead medium Id
    Bool_t            fOldGeometry; ///< Option for geometry definition

  ClassDef(BETAG4Application,1)  //Interface to MonteCarlo application
};

// inline functions

inline BETAG4Application* BETAG4Application::Instance()
{ 
  /// \return The MC application instance
  return (BETAG4Application*)(TVirtualMCApplication::Instance());
}

inline void BETAG4Application::SetOldGeometry(Bool_t oldGeometry)
{ 
  /// Select old geometry definition (via TVirtualMC)
  fOldGeometry = oldGeometry; 
}



#endif

