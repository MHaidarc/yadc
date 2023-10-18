#include <GL/glut.h>
#include <math.h>
#define P3 3 * M_PI / 2

float playerX, playerY, playerDeltaX, playerDeltaY, playerAngle;

int mapX = 8, mapY = 8, mapS = 64;
int map[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 1, 0, 0, 1, 
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 0, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 0, 1,
    1, 0, 0, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

float findDistance(float ax, float ay, float bx, float by, float ang) {
  return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void castRays() {
  int ray, mx, my, mp, depthOfField;
  float rayX, rayY, rayAngle, xOffset, yOffset;
  rayAngle = playerAngle;

  for (ray = 0; ray < 1; ray++) {
    depthOfField = 0;
    float disH = 1000000, hx = playerX, hy = playerY;
    float aTan = -1 / tan(rayAngle);

    if (rayAngle > M_PI) {
      rayY = (((int)playerY >> 6) << 6) - 0.0001;
      rayX = (playerY - rayY) * aTan + playerX;
      yOffset = -64;
      xOffset = -yOffset * aTan;
    }

    if (rayAngle < M_PI) {
      rayY = (((int)playerY >> 6) << 6) + 64;
      rayX = (playerY - rayY) * aTan + playerX;
      yOffset = 64;
      xOffset = -yOffset * aTan;
    }

    if (rayAngle == 0 || rayAngle == M_PI) {
      rayX = playerX;
      rayY = playerY;
      depthOfField = 8;
    }

    while (depthOfField < 8) {
      mx = (int)(rayX) >> 6;
      my = (int)(rayY) >> 6;
      mp = my * mapX + mx;

      if (mp > 0 && mp < mapX * mapY && map[mp] == 1) {
        depthOfField = 8;
        hx = rayX;
        hy = rayY;
        disH = findDistance(playerX, playerY, hx, hy, rayAngle);

      } else {
        rayX += xOffset;
        rayY += yOffset;
        depthOfField += 1;
      }
    }

    depthOfField = 0;
    float disV = 1000000, vx = playerX, vy = playerY;

    float nTan = -tan(rayAngle);
    if (rayAngle > M_PI_2 && rayAngle < P3) {
      rayX = (((int)playerX >> 6) << 6) - 0.0001;
      rayY = (playerX - rayX) * nTan + playerY;
      xOffset = -64;
      yOffset = -xOffset * nTan;
    }

    if (rayAngle < M_PI_2 || rayAngle > P3) {
      rayX = (((int)playerX >> 6) << 6) + 64;
      rayY = (playerX - rayX) * nTan + playerY;
      xOffset = 64;
      yOffset = -xOffset * nTan;
    }

    if (rayAngle == 0 || rayAngle == M_PI) {
      rayX = playerX;
      rayY = playerY;
      depthOfField = 8;
    }

    while (depthOfField < 8) {
      mx = (int)(rayX) >> 6;
      my = (int)(rayY) >> 6;
      mp = my * mapX + mx;

      if (mp > 0 && mp < mapX * mapY && map[mp] == 1) {
        vx = rayX;
        vy = rayY;
        disV = findDistance(playerX, playerY, vx, vy, rayAngle);
        depthOfField = 8;
      } else {
        rayX += xOffset;
        rayY += yOffset;
        depthOfField += 1;
      }
    }
    if (disV < disH) {
      rayX = vx;
      rayY = vy;
    }
    
    if (disV > disH) {
      rayX = hx;
      rayY = hy;
    }

    glColor3f(1, 0, 0);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2i(playerX, playerY);
    glVertex2i(rayX, rayY);
    glEnd();
  }
}

void drawMap2D() {
  int x, y, xOffset, yOffset;
  for (y = 0; y < mapY; y++) {
    for (x = 0; x < mapX; x++) {
      if (map[y * mapX + x] == 1)
        glColor3f(1, 1, 1);
      else
        glColor3f(0, 0, 0);

      xOffset = x * mapS;
      yOffset = y * mapS;
      glBegin(GL_QUADS);
      glVertex2i(xOffset + 1, yOffset + 1);
      glVertex2i(xOffset + 1, yOffset + mapS - 1);
      glVertex2i(xOffset + mapS - 1, yOffset + mapS - 1);
      glVertex2i(xOffset + mapS - 1, yOffset + 1);
      glEnd();
    }
  }
}

void drawPlayer() {
  glColor3f(1, 1, 0);
  glPointSize(8);
  glBegin(GL_POINTS);
  glVertex2i(playerX, playerY);
  glEnd();

  glLineWidth(3);
  glBegin(GL_LINES);
  glVertex2i(playerX, playerY);
  glVertex2i(playerX + playerDeltaX * 5, playerY + playerDeltaY * 5);
  glEnd();
}

void buttons(unsigned char key, int x, int y) {
  if (key == 'a') {
    playerAngle -= 0.1;
    if (playerAngle < 0) {
      playerAngle += 2 * M_PI;
    }
    playerDeltaX = cos(playerAngle) * 5;
    playerDeltaY = sin(playerAngle) * 5;
  }

  if (key == 'd') {
    playerAngle += 0.1;
    if (playerAngle > 2 * M_PI) {
      playerAngle -= 2 * M_PI;
    }
    playerDeltaX = cos(playerAngle) * 5;
    playerDeltaY = sin(playerAngle) * 5;
  }

  if (key == 'w') {
    playerX += playerDeltaX;
    playerY += playerDeltaY;
  }

  if (key == 's') {
    playerX -= playerDeltaX;
    playerY -= playerDeltaY;
  }

  glutPostRedisplay();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawMap2D();
  castRays();
  drawPlayer();
  glutSwapBuffers();
}

void init() {
  glClearColor(0.3, 0.3, 0.3, 0);
  gluOrtho2D(0, 1024, 512, 0);
  playerX = 300;
  playerY = 300;
  playerDeltaX = cos(playerAngle) * 5;
  playerDeltaY = sin(playerAngle) * 5;
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(1024, 512);
  glutCreateWindow("Doom");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(buttons);
  glutMainLoop();

  return 0;
}
