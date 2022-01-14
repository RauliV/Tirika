// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <memory>

// Types for IDs
using PlaceID = long long int;
using AreaID = long long int;
using Name = std::string;
using WayID = std::string;

// Return values for cases where required thing was not found
PlaceID const NO_PLACE = -1;
AreaID const NO_AREA = -1;
WayID const NO_WAY = "!!No way!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Enumeration for different place types
// !!Note since this is a C++11 "scoped enumeration", you'll have to refer to
// individual values as PlaceType::SHELTER etc.
enum class PlaceType { OTHER=0, FIREPIT, SHELTER, PARKING, PEAK, BAY, AREA, NO_TYPE };

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;



// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();
    // Estimate of performance: O(1)
    // Short rationale for estimate: Complexity of "size" is constant
    int place_count();

    // Estimate of performance: O(n)
    // Short rationale for esti mate: 2 x O(n) Clearing each element of both maps.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Push_back O(1)  * n
    std::vector<PlaceID> all_places();

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find and insert unord map O(n) -> T(1)
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find and in dex unordered map, both O(n)->T(1)
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find and index unord map
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlog n) ≈ Θ(lo g n)
    // Short rationale for estimate: insert map O(log n) * index unord map
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance: O(nlog n)
    // Short rationale for estimate: std::sort used
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance: O(n^2) ≈ Θ(n)
    // Short rationale for estimate: index n times x O(1) push_back
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance: O(n^2) ≈ Θ(n)
    // Short rationale for estimate: index n times x O(1) push_back
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find + index unord map
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find + index unord map
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find and insert unord map
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find unord map
    Name get_area_name(AreaID id);

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find unord map
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: n * push_back O(1)
    std::vector<AreaID> all_areas();

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find + index. Unordered map
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: O(n^2) ≈ Θ(n)
    // Short rationale for estimate: find + n * index
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance: -
    // Short rationale for estimate: -
    void creation_finished();

    // Estimate of performance: O(n)
    // Short rationale for estimate: clear, recursive loop n * O(1) = O(n)
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance: nlog n
    // Short rationale for estimate: using "sortCoord" -method, O(nlog n)
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate:find (n≈1) + erase (n≈1)
    bool remove_place(PlaceID id);

    // Estimate of performance: O(n^2) ≈ Θ(1)
    // Short rationale for estimate: O(n * n) ≈ Θ(1*1)
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);


    // Phase 2 operations

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find and insert unord map O(n) -> T(1)
    std::vector<WayID> all_ways();

    // Estimate of performance: O(n) ≈ Θ(1) x2, add ways and crossroads
    // Short rationale for estimate: find and index unordered map, both O(n)->T(1)
    bool add_way(WayID id, std::vector<Coord> coords);

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find and index unordered map, both O(n)->T(1)
    std::vector<std::pair<WayID, Coord>> ways_from(Coord xy);

    // Estimate of performance: O(n) ≈ Θ(1)
    // Short rationale for estimate: find and index unordered map, both O(n)->T(1)
    std::vector<Coord> get_way_coords(WayID id);

    // Estimate of performance: O(n)
    // Short rationale for esti mate: 2 x O(n) Clearing each element of both maps.
    void clear_ways();

    // Estimate of performance: nlog n
    // Short rationale for estimate: 4 x n + 1 + n * log n
    std::vector<std::tuple<Coord, WayID, Distance>> route_any(Coord fromxy, Coord toxy);



    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_way(WayID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_least_crossroads(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID>> route_with_cycle(Coord fromxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_shortest_distance(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_ways();

private:

    const Coord ORIGO{0,0};

    struct placeCoordItem
    {
        PlaceID myPlaceId;
        Coord   myPlaceCoordA;
        Coord   myPlaceCoordB;
        double distance = 0;

        bool operator<(const placeCoordItem& a) const
        {
            if (distance == a.distance)
            {
                return myPlaceCoordA.y < a.myPlaceCoordB.y;
            }
            return  distance < a.distance;
        }
    };


    struct Way
    {
      Distance pituus = 0;
      std::vector<Coord> coordinates = {};
    };

    enum class Color { WHITE, GRAY, BLACK };


    struct Crossroad
    {
        Distance matka = 0;
        Color vari = Color::WHITE;
        Coord pii;
        Coord myCoord;
        std::vector<std::pair<WayID, Coord>> waysTo = {};
    };


    struct Place
    {
        Name pName = "";
        PlaceType pType = {};
        Coord pCoord = {};

    };

    struct Area
    {
        Name aName = "";
        std::vector<Coord> aCoords = {};
        AreaID uberArea = NO_AREA;
        std::vector<AreaID> subArea = {};
    };

    std::unordered_map<WayID, std::shared_ptr<Way>> Ways = {};
    std::unordered_map<Coord, std::shared_ptr<Crossroad>, CoordHash> Crossroads = {};
    std::unordered_map<PlaceID, std::shared_ptr<Place>> Places = {};
    std::unordered_map<AreaID, std::shared_ptr<Area>> Areas = {};
    std::vector<AreaID> allSubareas_;
    std::vector<Coord> previousNodes_ = {};
    std::vector<std::tuple<Coord, WayID, Distance>> route_ = {};
    void rollback (Coord cr);

    void add_route(Coord node);
    Distance calcWayLenght(WayID way);
    void add_crossroads(WayID way);

    WayID findAWay (Coord A, Coord B);

    void BFS (Coord startingPoint, Coord endingPoint);

    double calcDistance(const Coord& a, const Coord& b);
    std::vector<PlaceID> sortCoord (const std::vector<PlaceID>& listToSort, const Coord& fromWhere);
    void makeLikeATree(AreaID);
};


#endif // DATASTRUCTURES_HH
