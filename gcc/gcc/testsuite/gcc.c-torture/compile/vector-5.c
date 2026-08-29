#define vector __attribute__((vector_size(16) ))

typedef struct Position3 {
 vector float v;
}Position3;
Position3 lastOnGroundPos;
void CreatePickupOnDeath() {
  Position3 pos = lastOnGroundPos;
  ((float*)&pos.v)[0] += 1.0f;
  lastOnGroundPos = pos;
}

void CreatePickupOnDeath1() {
  Position3 pos = lastOnGroundPos;
  ((float*)&pos.v)[2] += 1.0f;
  lastOnGroundPos = pos;
}

