
module Chat {

interface RoomFactory;
interface Room;

interface User
{
  string getName();
  void receiveMessage(string message, User* sender, Room* room);
  void receivePrivateMessage(string message, User* sender); 
};

sequence<User*> userList;
exception UserNotExists{};
exception UserAlreadyExists{};

sequence<Room*> roomList;
exception RoomNotExists{};
exception RoomAlreadyExists{};

interface Server
{
  Room* getRoom(string name);
  roomList getRooms();
  Room* addRoom(string name) throws RoomAlreadyExists;
  void removeRoom(string name) throws RoomNotExists;
  void addNewFactory(RoomFactory* factory);
};

interface Room
{
  userList presentUsers();
  void sendMessage(string message, User* sender);
  void addUser(User* who);
  void removeUser(User* who) throws UserNotExists;
  string getName();
};


interface RoomFactory
{
  Room* addRoom(string name) throws RoomAlreadyExists;
  void removeRoom(string name) throws RoomNotExists;
  int getCPULoad();
};

};
