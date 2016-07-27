
#include "BVFile.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace soci;

BVBoard::BVBoard(const string * fpath, const string * driver_name) {
  // start DB session
  if (open_db_file_(fpath, driver_name)) {
    // pulling all data into vectors
    fetch_pin_data_(&this->sql_);
    //fetch_nail_data_(&this->sql_);
    //fetch_layout_data_(&this->sql_);
  }
  close_db_session_();

  {
    // // TODO: some debugging code to remove here
    // cout << "### BV Board Stats ###" << endl;
    // cout << "# " << this->pins.size() << " pins" << endl;
    // cout << "# " << this->nails.size() << " nails" << endl;
    // cout << "# " << this->layout.size() << " points" << endl;
    //
    // cout << "### First Line Outputs ###" << endl;
    // Pin p = pins[0];
    // cout  << p.part_name << p.on_top << p.pin_number
    //       << p.pin_name << p.x << p.y
    //       << p.layer << p.net_name << endl;
    //
    // Nail n = nails[0];
    // cout  << n.name << n.x << n.y
    //       << n.type << n.grid << n.on_top
    //       << n.net_number << n.net_name << endl;
    //
    // Layout l = layout[0];
    // cout  << l.x << l.y << l.r << l.unique_group << endl;
  }
}

bool BVBoard::open_db_file_(
  const string * fpath, const string * driver_name) {
  if (fpath->empty())
    return false;

  // TODO: check path vaildity

  string driver = driver_name->empty() ? kStdAccessDriver : *driver_name;

  try {
    // NOTE: alternative way to specify file via DSN file:
    //session sql(odbc, "filedsn=D:/tmp/filo.dsn;");
    this->sql_.open(odbc,
      "DRIVER=" + driver
      + ";FIL=MS Access;DBQ=" + *fpath + ";");
    return true;
  } catch (odbc_soci_error const& e) {
    // FIXME: use some logging util instead of CERR
    cerr << "ODBC Error Code: " << e.odbc_error_code() << endl
         << "Native Error Code: " << e.native_error_code() << endl
         << "SOCI Message: " << e.what() << std::endl
         << "ODBC Message: " << e.odbc_error_message() << endl;
  } catch (exception const &e) {
    cerr << "Error: " << e.what() << endl;
  }
  return false;
}

void BVBoard::close_db_session_() {
  this->sql_.close();
}

void BVBoard::fetch_pin_data_(session * sql) {
  rowset<int> rs = (sql->prepare << "SELECT Group FROM Layout");
  copy(rs.begin(), rs.end(), ostream_iterator<int>(cout, "\n"));

  // for (auto& ro : row_set) {
  //   ro.get<double>(0);
  //   cout << "X ";
  // }
  //
  //     // Pin p = {"", "", false, -1, 0.0f, 0.0f, -1, ""};
  //     //
  //     // p.part_name = ro.get<string>(0);
  //     // p.on_top = (ro.get<string>(1) == "(T)") ? true : false;
  //     // p.pin_number = ro.get<int>(2);
  //     // p.pin_name = ro.get<string>(3);
  //     // p.x = ro.get<double>(4);
  //     // p.y = ro.get<double>(5);
  //     // p.layer = ro.get<int>(6);
  //     // p.net_name = ro.get<string>(7);
  //     //
  //     // this->pins.push_back(p);
  //}
}

void BVBoard::fetch_nail_data_(session * sql) {
  rowset<row> row_set = (sql->prepare <<
    " SELECT Nail, X, Y, Type, Grid, TB, NET, NetName, VirtualPinVia\
      FROM Nail\
      ORDER BY Nail.NetName;");

  for (auto& ro : row_set) {
      Nail n = {"", 0.0f, 0.0f, -1, "", false, "", ""};

      n.name = ro.get<string>(0);
      n.x = ro.get<double>(1);
      n.y = ro.get<double>(2);
      n.type = ro.get<int>(3);
      n.grid = ro.get<string>(4);
      n.on_top = (ro.get<string>(5) == "(T)") ? true : false;
      n.net_number = ro.get<string>(6);
      n.net_name = ro.get<string>(7);
      // n.virtual_pin = to.get<string>(8);
      this->nails.push_back(n);
  }
}

void BVBoard::fetch_layout_data_(session * sql) {
  rowset<row> row_set = (sql->prepare <<
    " SELECT X, Y, R, Group\
      FROM Layout\
      ORDER BY Layout.X;");

  for (auto& ro : row_set) {
      Layout l = {0.0f, 0.0f, -1, -1};

      l.x = ro.get<double>(0);
      l.y = ro.get<double>(1);
      l.r = ro.get<double>(2);
      l.unique_group = ro.get<int>(3);

      this->layout.push_back(l);
  }
}
