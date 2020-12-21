RENDERMANDIR=/usr
BLENDER=/usr/bin/blender
QTDIR:=$(shell ls -d /usr/lib/qt-3*)

MOC=$(QTDIR)/bin/moc
IFLAGS=-I$(QTDIR)/include -I../include -I$(RENDERMANDIR)/include/aqsis
LFLAGS=-L$(QTDIR)/lib -L/usr/X11R6/lib -L../lib -lqt-mt -L$(RENDERMANDIR)/lib -lri2rib

%.o : %.cc
	g++ -g -c $*.cc -I. $(IFLAGS)

%.moc: %.cc
	$(MOC) $*.cc -o $*.moc

%.moh: ../include/%.h
	$(MOC) ../include/$*.h -o $*.moh

all: ObjectFlock

ObjectFlock.o: ObjectFlock.moc

ObjectFlock: ObjectFlock.o Mesh.o Geometry.o BoidFlock.o Boid.o Timer.o ImageBuffer.o Terrain.o Water.o TextureMap.o Critter.o Skeleton.o Movements.o BezierSpline.o Curve.o
	g++ -g $+ -o ObjectFlock -lGL -lGLU $(LFLAGS)

