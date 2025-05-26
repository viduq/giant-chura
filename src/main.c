#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

/*
  this demo is mostly taken from @javidx9's excellent tutorial
  on isometric tiles on youtube https://www.youtube.com/watch?v=ukkbNKTgf5U
*/

typedef struct {
  int x, y;
} Vector2i;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 2500;
  const int screenHeight = 1500;

  Vector2i vWorldSize = {15, 15};
  Vector2i vTileSize = {128, 64};
  Vector2i vOrigin = {10, 1};
  int *pWorld = (int *)calloc(vWorldSize.x * vWorldSize.y, sizeof(int));

  char *tiles_file = "src/resources/new_tilemap.png";
  char *cheating_tile = "src/resources/new_cheating_tile.png";

  InitWindow(screenWidth, screenHeight, "raylib isometric tiles demo");

  // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
  Texture2D tiles = LoadTexture(tiles_file);  // Texture loading
  Image img = LoadImage(cheating_tile);       // Load as image for getting single pixels
  Color *pixels = LoadImageColors(img);

  int tileWidth = 128;
  int tileHeight = 64;

  int tileX = 5;
  int tileY = 3;

  Rectangle emptyTile = {0, 0, 128, 64};
  Rectangle selectedTile = {1 * 128, 0, 128, 64};
  Rectangle grassTile = {2 * 128, 0, 128, 64};
  Rectangle sandTile = {3 * 128, 0, 128, 64};
  Rectangle waterTile = {4 * 128, 0, 128, 64};

  Rectangle treeTile = {0 * 128, 2 * 64, 128, 128};

  // Rectangle waterTile = {3 * 40, 1 * 40, 40, 20};
  // Rectangle christmasTree = {0, 1 * 20, 40, 40};
  // Rectangle appleTree = {1 * 40, 1 * 20, 40, 40};

  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------

    Vector2 MousePositionF = GetMousePosition();
    Vector2i vMousePositionI = {(MousePositionF.x), (MousePositionF.y)};
    Vector2i vCell = {vMousePositionI.x / vTileSize.x, vMousePositionI.y / vTileSize.y};
    Vector2i vOffset = {(vMousePositionI.x) % vTileSize.x, vMousePositionI.y % vTileSize.y};
    Color color = GetPixelColor(&pixels[vOffset.y * img.width + vOffset.x], PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    Vector2i vSelected = {(vCell.y - vOrigin.y) + (vCell.x - vOrigin.x), (vCell.y - vOrigin.y) - (vCell.x - vOrigin.x)};
    if (color.r == 255 && color.g == 0) {
      vSelected.x -= 1;
    }
    if (color.b == 255) {
      vSelected.y += 1;
    }
    if (color.g == 255 && color.r == 0) {
      vSelected.y -= 1;
    }
    if (color.g == 255 && color.r == 255) {
      vSelected.x += 1;
    }

    float selectedX = (vSelected.x - vSelected.y) * (tileWidth / 2) + (vOrigin.x * vTileSize.x);
    float selectedY = (vSelected.x + vSelected.y) * (tileHeight / 2) + (vOrigin.y * vTileSize.y);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      // printf("vorher: %d\n", pWorld[vSelected.y * vWorldSize.x + vSelected.x]);
      (pWorld[vSelected.y * vWorldSize.x + vSelected.x]++) % 8;
    }

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText(TextFormat("mouse pos: %d,%d", vMousePositionI.x, vMousePositionI.y), 0, 0, 20, BLACK);
    DrawText(TextFormat("Cell: %d,%d", vCell.x, vCell.y), 0, 30, 20, BLACK);
    DrawText(TextFormat("Selected: %d,%d", vSelected.x, vSelected.y), 0, 60, 20, BLACK);

    // DrawRectangle(vCell.x * vTileSize.x, vCell.y * vTileSize.y, vTileSize.x, vTileSize.y, RED);

    for (int y = 0; y < vWorldSize.y; y++) {
      for (int x = 0; x < vWorldSize.x; x++) {
        float screenX = (x - y) * (tileWidth / 2) + (vOrigin.x * vTileSize.x);
        float screenY = (x + y) * (tileHeight / 2) + (vOrigin.y * vTileSize.y);
        Rectangle destRec = {screenX, screenY, tileWidth, tileHeight};

        switch (pWorld[y * vWorldSize.x + x]) {
          case 0:
            DrawTexturePro(tiles, emptyTile, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            break;
          case 1:
            DrawTexturePro(tiles, grassTile, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            break;
          case 2:
            DrawTexturePro(tiles, sandTile, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            break;
          case 3:
            DrawTexturePro(tiles, waterTile, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            break;
          case 4:
            destRec.y -= tileHeight;
            destRec.height = tileHeight * 2;
            DrawTexturePro(tiles, treeTile, destRec, (Vector2){0, 0}, 0.0f, WHITE);
            break;
          // case 5:
          //   destRec.y -= tileHeight;
          //   destRec.height = tileHeight * 2;
          //   DrawTexturePro(tiles, appleTree, destRec, (Vector2){0, 0}, 0.0f, WHITE);
          //   break;
          default:
            pWorld[y * vWorldSize.x + x] = 0;
        }
      }
    }

    Rectangle destRecSelected = {selectedX, selectedY, tileWidth, tileHeight};
    DrawTexturePro(tiles, selectedTile, destRecSelected, (Vector2){0, 0}, 0.0f, GREEN);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}