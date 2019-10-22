typedef struct {
  unsigned char version = 0;
  bool wifiConfigured = false;
} mainConfig_t;

mainConfig_t mainConfig;

bool wifiVerified = false;