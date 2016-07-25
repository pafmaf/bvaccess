#include <string>
#include <vector>
#include <cstddef>

#include <soci/soci.h>
#include <soci/odbc/soci-odbc.h>

using namespace std;
using namespace soci;

// There's a good chance one of those drivers is presinstalled. (?)
const string kStdWindowsAccessDriver = "Microsoft Access Driver (*.mdb)";
const string kStdUnixAccessDriver = "";
const string kStdLinuxAccessDriver = "";

#ifdef _WIN32
  const string kStdAccessDriver = kStdWindowsAccessDriver;
#elif __linux__
  const string kStdAccessDriver = kStdLinuxAccessDriver;
#elif __APPLE__
  const string kStdAccessDriver = kStdUnixAccessDriver;
#endif

struct Pin {
  // e.g. Q3820
  string part_name;

  // Pin name referring to part/component e.g. [GATE, DRAIN, AA19, AB22]
  string pin_name;

  // true if on top, false else
  bool on_top;

  // Number of pin on component
  int pin_number;

  double x;
  double y;

  // mostly relating to bottom (0), top (1) or chassis (2)
  int layer;

  // e.g. [GND, FSB_D_L<58>, GND_CHASSIS_LVDS, (NC)]
  string net_name;
};

struct Layout {
  double x, y, r;

  // group a point is belonging to
  // (probably support for multiple outlines / layouts)
  int unique_group;
};

struct Nail {
  // mostly just "0"
  string name;

  double x, y;

  // only seen values (1,2,3)
  // TODO: not sure what those are
  int type;

  // e.g. (A3, H1), probably grid coordinates on board
  string grid;

  // true if on top, false else
  bool on_top;

  // string
  string net_number;

  // unique string, same as in pin.net_name
  string net_name;
};

class BVBoard {
public:
  vector<Layout> layout;
  vector<Pin> pins;
  vector<Nail> nails;

  // fetching data from BV DB file specified by fpath
  // accessing the DB via driver_name, if "" trying OS standard driver
  BVBoard(const string * fpath, const string * driver_name);
private:
  // currecnt sql session
  session sql_;

  // opens a new SQL session kept in sql_ for file and specified driver
  // returns true on success
  bool open_db_file_(const string * fpath, const string * driver_name);

  // closes db session sql_
  void close_db_session_();

  // fetches layout sorted by X value
  void fetch_layout_data_(session * sql);

  // fetches nails sorted alphabetically by net_name
  void fetch_nail_data_(session * sql);

  // fetches pins sorted alphabetically by part_name
  void fetch_pin_data_(session * sql);

};
