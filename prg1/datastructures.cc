 // Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <map>
#include <set>
#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

int Datastructures::place_count()
{
    return Places.size();
}

void Datastructures::clear_all()
{
    Places = {};
    Areas = {};
}

std::vector<PlaceID> Datastructures::all_places()
{
    std::vector<PlaceID> aPlaces = {};
    for (auto const& place : Places)
    {
        aPlaces.push_back(place.first);
    }
    return aPlaces;
}


bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    if (Places.find(id) != Places.end())
    {
        return false;
    }
    else
    {
        std::shared_ptr<Place> newPlace = std::make_shared<Place> ();
        newPlace->pName = name;
        newPlace->pCoord = xy;
        newPlace->pType = type;
        Places.insert({id, newPlace});
        return true;
    }
}


std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    if (Places.find(id) == Places.end())
    {
        return {NO_NAME, PlaceType::NO_TYPE};
    }
    else
    {
        std::pair<Name, PlaceType> nameType = {Places.at(id)->pName,
                                               Places.at(id)->pType};
        return nameType;
    }
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    if (Places.find(id) == Places.end())
    {
        return NO_COORD;
    }
    else
    {
        Coord placeCoord = Places.at(id)->pCoord;
        return placeCoord;
    }
}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    if (Areas.find(id) != Areas.end())
    {
        return false;
    }

    std::shared_ptr<Area> newArea = std::make_shared<Area> ();
    newArea->aName = name;
    newArea->aCoords = coords;
    Areas.insert({id, newArea});
    return true;
}


Name Datastructures::get_area_name(AreaID id)
{
    if (Areas.find(id) == Areas.end())
    {
        return NO_NAME;
    }

    return Areas.at(id)->aName;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    if (Areas.find(id) == Areas.end())
    {
        return {NO_COORD};
    }

    return Areas.at(id)->aCoords;
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}


std::vector<PlaceID> Datastructures::places_alphabetically()
{
    std::vector<PlaceID> returnVector = {};
    std::map<Name, PlaceID> NameId = {};
    for (auto const& place : Places) //T->(1)
    {
        NameId.insert({place.second->pName, place.first}); // O(log n)
    }

    for (auto const& it : NameId)
    {
        returnVector.push_back(it.second); // O(1)
    }

    return returnVector;
}


double Datastructures::calcDistance(const Coord& a, const Coord& b)
{
    int discX = (b.x-a.x)*(b.x-a.x);
    int discY = (b.y-a.y)*(b.y-a.y);

    double distance = sqrt(discX+discY);
    return distance;
}


std::vector<PlaceID> Datastructures::sortCoord (
        const std::vector<PlaceID>& listToSort, const Coord& fromWhere)

{
    std::vector<PlaceID> sortedList = {};
    std::vector<placeCoordItem> cItemList = {};

    for(auto const& item : listToSort) //index vector O(1)
    {
        double dist = calcDistance(fromWhere, get_place_coord(item));
        placeCoordItem coordItem;
        coordItem.distance = dist;
        coordItem.myPlaceCoordA = fromWhere;
        coordItem.myPlaceCoordB = Places.at(item)->pCoord;
        coordItem.myPlaceId = item;
        cItemList.push_back(coordItem); // O(1)
     }

    // lajitteluperusteoperaattori structin sisällä
    std::sort(cItemList.begin(), cItemList.end()); //O(nlog n)

    for (auto const& item : cItemList)
    {
        sortedList.push_back(item.myPlaceId); //O(1)
    }

    return sortedList;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    std::map<double, std::map<int, PlaceID>> coordOrder = {};
    return sortCoord(all_places(), ORIGO);
}


std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    std::vector<PlaceID> pNamevec = {};
    for (auto const& place : Places)
    {
        if (name == place.second->pName)
        {
            pNamevec.push_back(place.first); //O(1)
        }
    }
    return pNamevec;
}


std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    std::vector<PlaceID> pTypevec = {};
    for (auto const& place : Places)
    {
        if (type == place.second->pType)
        {
            pTypevec.push_back(place.first);
        }
    }
    return pTypevec;
}


bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    if (Places.find(id) == Places.end())
    {
        return false;
    }

    Places.at(id)->pName = newname;
    return true;
}


bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    if (Places.find(id) == Places.end())
    {
        return false;
    }

    Places.at(id)->pCoord = newcoord;
    return true;

}


std::vector<AreaID> Datastructures::all_areas()
{
    std::vector<AreaID> aAreas = {};
    for (auto const& area : Areas)
    {
        aAreas.push_back(area.first);
    }
    return aAreas;
}


bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    if ((Areas.find(id) == Areas.end()) or
        (Areas.find(parentid) == Areas.end()) or
        (Areas.at(id)->uberArea != NO_AREA))
    {
        return false;
    }

    //pitää lisätä seuraava yläalue ja ala-alueiden vektoriin
    Areas.at(id)->uberArea = parentid;
    Areas.at(parentid)->subArea.push_back(id);
    return true;
}


std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    if (Areas.find(id) == Areas.end())  //O(n) -> T(1)
    {
        return {NO_AREA};
    }

    std::vector<AreaID> uberAreaList = {};
    AreaID currentArea;
    if (Areas.at(id) ->uberArea == NO_AREA)
    {
        return uberAreaList;
    }
    else
    {

        currentArea = Areas.at(id)->uberArea;
    }         //O(n) -> T(1)




    //Edetään suoraviivaisesti hierarkiassa, kunnes ei enää yläalueita
    while (Areas.at(currentArea)->uberArea != NO_AREA)
    {
        uberAreaList.push_back(currentArea);    // O(1)
        currentArea = Areas.at(currentArea)->uberArea; //O(n) -> T(1)
    }

    uberAreaList.push_back(currentArea);        //O(1)
    return uberAreaList;
}


std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    std::vector<PlaceID> returnPlaces;
    if (type == PlaceType::NO_TYPE)
    {
        returnPlaces = sortCoord(all_places(), xy); //Sortcoord = O(nlog n)
    }
    else
    {
        std::vector<PlaceID> myTypePlaces = find_places_type(type);
        returnPlaces = sortCoord(myTypePlaces, xy);
    }

    if (returnPlaces.size() > 3)
    {
        returnPlaces.resize(3);
    }

    return  returnPlaces;
}


bool Datastructures::remove_place(PlaceID id)
{
    if (Places.find(id) == Places.end())
    {
        return false;
    }

    Places.erase(id);
    return true;
}


void Datastructures::makeLikeATree(AreaID id)
{
    //Check if this area has any subareas
    if (Areas.at(id)->subArea.size() != 0)
    {
        for (auto const& sibling : Areas.at(id)->subArea) //O(n) * O(1)
        {
            makeLikeATree(sibling);
        }
    }
    allSubareas_.push_back(id); //O(1)
}


std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    if (Areas.find(id) == Areas.end()) //O(log n)
    {
        return {NO_AREA};
    }

    allSubareas_ = {}; //O(n)
    makeLikeATree(id);  // O(n)
    allSubareas_.pop_back(); //O(1)

    return allSubareas_;
}


AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    if (Areas.find(id1) == Areas.end() or Areas.find(id2) == Areas.end()) //O(n) -> T(1)
    {
        return NO_AREA;
    }

    //Using previously implemented method
    for (auto const& area : subarea_in_areas(id1)) //   O(n^2) ≈ Θ(1)    n * n * O(n) -> T(1)
    {
        for ( unsigned long i = 0; i < subarea_in_areas(id2).size(); i++) //index size O(1)
        {
            if (area == subarea_in_areas(id2).at(i))
            {

                return area;
            }
        }
    }

    return NO_AREA;
}
