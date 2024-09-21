//----------------------------------Static CUBE

const int cubeSize = 28; //cube size
const int ls = 5; //centerX - (cubeSize / 2); //Line Start
const int le = 26; //centerX + (cubeSize / 2); //Line End

void drawCube() {
  // Clear both arrays
  memset(voxelGrid, 0, sizeof(voxelGrid));
  memset(Polar, 0, sizeof(Polar));

  //top
  drawLine3D(voxelGrid, ls, ls, ls, le, ls, ls);
  drawLine3D(voxelGrid, le, ls, ls, le, le, ls);
  drawLine3D(voxelGrid, le, le, ls, ls, le, ls);
  drawLine3D(voxelGrid, ls, le, ls, ls, ls, ls);

  //bottom
  drawLine3D(voxelGrid, ls, ls, le, le, ls, le);
  drawLine3D(voxelGrid, le, ls, le, le, le, le);
  drawLine3D(voxelGrid, le, le, le, ls, le, le);
  drawLine3D(voxelGrid, ls, le, le, ls, ls, le);

  //mid connections
  drawLine3D(voxelGrid, ls, ls, ls, ls, ls, le);
  drawLine3D(voxelGrid, le, ls, ls, le, ls, le);
  drawLine3D(voxelGrid, ls, le, ls, ls, le, le);
  drawLine3D(voxelGrid, le, le, ls, le, le, le);
}