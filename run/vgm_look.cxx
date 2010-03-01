// Demonstrates usage of "Default" geometry alias.

void vgm_look()
{
   TEveManager::Create();

   gEve->RegisterGeometryAlias("BETA", "VGM_test.root");

 gGeoManager = gEve->GetGeometryByAlias("BETA");
   
   TGeoNode* node1 = gGeoManager->GetTopNode();
   TEveGeoTopNode* its = new TEveGeoTopNode(gGeoManager, node1);
   gEve->AddGlobalElement(its);

   gEve->Redraw3D(kTRUE);
   
   
//     TGeoManager::Import("VGM_test.root");
//   //gGeoManager->DefaultColors();
//   gGeoManager->SetMaxVisNodes(5000);
//   //gGeoManager->SetVisLevel(4);
//   gGeoManager->GetVolume("World")->Draw("ogl");
//   new TBrowser;
}
