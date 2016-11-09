VPATH = src inc lib
CPPFLAGS = -I include -Wall -std=c++11

new: clean all

clean:
	rm -rf *.o */*.o *_app *.a

all: examples

examples: game_no_limit

main: libgame.a


# Create Library of shared files

libgame.a: libgame.a(DownStairsRoom.o) libgame.a(DropRoom.o) libgame.a(EmptyRoom.o) libgame.a(GameMaster.o) libgame.a(Inventory.o) libgame.a(Item.o) libgame.a(Map.o) libgame.a(Room.o) libgame.a(UpStairsRoom.o)

libgame.a(DownStairsRoom.o): DownStairsRoom.o
	$(AR) $(ARFLAGS) $@ $?
libgame.a(DropRoom.o): DropRoom.o
	$(AR) $(ARFLAGS) $@ $?
libgame.a(EmptyRoom.o): EmptyRoom.o
	$(AR) $(ARFLAGS) $@ $?
libgame.a(GameMaster.o): GameMaster.o
	$(AR) $(ARFLAGS) $@ $?
libgame.a(Inventory.o): Inventory.o
	$(AR) $(ARFLAGS) $@ $?
libgame.a(Item.o): Item.o
	$(AR) $(ARFLAGS) $@ $?
libgame.a(Map.o): Map.o
	$(AR) $(ARFLAGS) $@ $?
libgame.a(Room.o): Room.o
	$(AR) $(ARFLAGS) $@ $?
libgame.a(UpStairsRoom.o): UpStairsRoom.o
	$(AR) $(ARFLAGS) $@ $?
	
Game_No_limit.o: GameMaster.hpp
DownStairsRoom.o: DownStairsRoom.hpp
DropRoom.o: DropRoom.hpp
EmptyRoom.o: EmptyRoom.hpp
GameMaster.o: GameMaster.hpp
Inventory.o: Inventory.hpp
Map.o: Map.hpp Room.hpp DropRoom.hpp EmptyRoom.hpp DownStairsRoom.hpp UpStairsRoom.hpp
Room.o: Room.hpp
# Timer.o: Timer.hpp
UpStairsRoom.o: UpStairsRoom.hpp


