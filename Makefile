# Amiga Developement Studio

CFLAGS=

PROG = AmigaDevStudio
LDIR =../lib

LIBS=-lstdc++

SRCS = AQApplication.cpp \
       AQClipboard.cpp \
       AQAction.cpp \
       AQString.cpp \
       AQWidget.cpp \
       AQWindow.cpp \
       AQLabel.cpp \
       AQButton.cpp \
       AQDialog.cpp \
       AQIcon.cpp \
       AQLayout.cpp \
       AQListView.cpp \
       AQMenu.cpp \
       AQScrollBar.cpp \
       AQSplitter.cpp \
       AQTextEdit.cpp \
       AQTextDoc.cpp \
       AQLineEdit.cpp \
       AQStatusBar.cpp \
       AQMainWindow.cpp \
       DevStudio.cpp \
       Project.cpp \
       main.cpp 

_OBJS = $(SRCS)
__OBJS = $(patsubst %.c,%.cpp, $(_OBJS))
RELOBJS = $(patsubst %.cpp,release/%.o, $(__OBJS))
DEBOBJS = $(patsubst %.cpp,debug/%.o, $(__OBJS))
 
release: $(RELOBJS)
	g++ -o release/$(PROG) $(CFLAGS) -noixemul $(LIBS) $(RELOBJS)

debug: $(DEBOBJS)
	g++ -o debug/$(PROG) -g $(CFLAGS) $(LIBS) $(DEBOBJS)

release/%.o: %.cpp
	g++ -o $@ -I. $(CFLAGS) -c $*.cpp

debug/%.o: %.cpp
	g++ -o $@ -I. -g $(CFLAGS) -c $*.cpp

.PHONY: clean depend

clean:
	rm -f debug/* release/*.o release/$(PROG)

depend:
	makedepend -- $(CFLAGS) -I/gg/os-include -I/gg/include/g++-3 -I/gg/m68k-amigaos/include/ -I/gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/ -I. -- $(SRCS)

# End of makefile
 # DO NOT DELETE

AQApplication.o: AQApplication.h /gg/include/g++-3/vector.h
AQApplication.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQApplication.o: /gg/include/g++-3/stl_config.h
AQApplication.o: /gg/m68k-amigaos/include/_G_config.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQApplication.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQApplication.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQApplication.o: /gg/include/g++-3/stl_function.h
AQApplication.o: /gg/include/g++-3/iostream.h /gg/include/g++-3/streambuf.h
AQApplication.o: /gg/include/g++-3/libio.h /usr/include/sys/cdefs.h
AQApplication.o: /gg/os-include/machine/cdefs.h
AQApplication.o: /gg/include/g++-3/stl_iterator.h
AQApplication.o: /gg/include/g++-3/type_traits.h
AQApplication.o: /gg/include/g++-3/stl_construct.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQApplication.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQApplication.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQApplication.o: /gg/os-include/machine/ansi.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQApplication.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQApplication.o: /gg/os-include/machine/types.h
AQApplication.o: /gg/os-include/machine/endian.h
AQApplication.o: /gg/include/g++-3/stl_uninitialized.h
AQApplication.o: /gg/include/g++-3/alloc.h /gg/include/g++-3/stl_alloc.h
AQApplication.o: /gg/m68k-amigaos/include/assert.h
AQApplication.o: /gg/include/g++-3/stl_vector.h AQKernel.h
AQApplication.o: /usr/include/stdio.h /gg/include/g++-3/algorithm
AQApplication.o: /gg/include/g++-3/stl_tempbuf.h /gg/include/g++-3/stl_algo.h
AQApplication.o: /gg/include/g++-3/stl_heap.h /gg/os-include/exec/types.h
AQApplication.o: /gg/os-include/intuition/intuition.h
AQApplication.o: /gg/os-include/graphics/gfx.h /gg/os-include/graphics/clip.h
AQApplication.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQApplication.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQApplication.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQApplication.o: /gg/os-include/graphics/view.h
AQApplication.o: /gg/os-include/graphics/copper.h
AQApplication.o: /gg/os-include/graphics/gfxnodes.h
AQApplication.o: /gg/os-include/graphics/monitor.h
AQApplication.o: /gg/os-include/graphics/displayinfo.h
AQApplication.o: /gg/os-include/graphics/modeid.h
AQApplication.o: /gg/os-include/utility/tagitem.h
AQApplication.o: /gg/os-include/hardware/custom.h
AQApplication.o: /gg/os-include/graphics/rastport.h
AQApplication.o: /gg/os-include/graphics/layers.h
AQApplication.o: /gg/os-include/graphics/text.h
AQApplication.o: /gg/os-include/devices/inputevent.h
AQApplication.o: /gg/os-include/devices/timer.h /gg/os-include/exec/io.h
AQApplication.o: /gg/os-include/intuition/screens.h
AQApplication.o: /gg/os-include/intuition/iobsolete.h
AQApplication.o: /gg/os-include/intuition/preferences.h
AQApplication.o: /gg/os-include/libraries/gadtools.h
AQApplication.o: /gg/os-include/proto/exec.h /gg/os-include/exec/memory.h
AQApplication.o: /gg/os-include/exec/devices.h
AQApplication.o: /gg/os-include/exec/libraries.h
AQApplication.o: /gg/os-include/clib/exec_protos.h
AQApplication.o: /gg/os-include/inline/exec.h /gg/os-include/inline/macros.h
AQApplication.o: /gg/os-include/inline/stubs.h /gg/os-include/dos/dos.h
AQApplication.o: /gg/os-include/intuition/classes.h
AQApplication.o: /gg/os-include/intuition/classusr.h
AQApplication.o: /gg/os-include/libraries/commodities.h
AQApplication.o: /gg/os-include/proto/alib.h
AQApplication.o: /gg/os-include/clib/alib_protos.h
AQApplication.o: /gg/os-include/devices/keymap.h
AQApplication.o: /gg/os-include/graphics/graphint.h
AQApplication.o: /gg/os-include/inline/alib.h /gg/os-include/proto/graphics.h
AQApplication.o: /gg/os-include/graphics/scale.h
AQApplication.o: /gg/os-include/clib/graphics_protos.h
AQApplication.o: /gg/os-include/graphics/gels.h
AQApplication.o: /gg/os-include/graphics/regions.h
AQApplication.o: /gg/os-include/graphics/sprite.h
AQApplication.o: /gg/os-include/hardware/blit.h
AQApplication.o: /gg/os-include/inline/graphics.h
AQApplication.o: /gg/os-include/proto/intuition.h
AQApplication.o: /gg/os-include/clib/intuition_protos.h
AQApplication.o: /gg/os-include/intuition/cghooks.h
AQApplication.o: /gg/os-include/inline/intuition.h
AQApplication.o: /gg/os-include/proto/gadtools.h
AQApplication.o: /gg/os-include/clib/gadtools_protos.h
AQApplication.o: /gg/os-include/inline/gadtools.h
AQApplication.o: /gg/os-include/proto/layers.h
AQApplication.o: /gg/os-include/clib/layers_protos.h
AQApplication.o: /gg/os-include/inline/layers.h AQWindow.h AQString.h
AQApplication.o: AQLayout.h ./AQWidget.h ./AQObject.h ./AQClipboard.h
AQClipboard.o: AQString.h /gg/os-include/dos/dos.h AQKernel.h ./AQClipboard.h
AQAction.o: AQString.h /gg/os-include/dos/dos.h AQKernel.h AQAction.h
AQAction.o: ./AQObject.h /gg/include/g++-3/vector.h
AQAction.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQAction.o: /gg/include/g++-3/stl_config.h
AQAction.o: /gg/m68k-amigaos/include/_G_config.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQAction.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQAction.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQAction.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQAction.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQAction.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQAction.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQAction.o: /gg/include/g++-3/stl_construct.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQAction.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQAction.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQAction.o: /gg/os-include/machine/ansi.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQAction.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQAction.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQAction.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQAction.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQAction.o: /gg/include/g++-3/stl_vector.h
AQAction.o: /gg/os-include/intuition/intuition.h
AQAction.o: /gg/os-include/graphics/gfx.h /gg/os-include/graphics/clip.h
AQAction.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQAction.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQAction.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQAction.o: /gg/os-include/graphics/view.h /gg/os-include/graphics/copper.h
AQAction.o: /gg/os-include/graphics/gfxnodes.h
AQAction.o: /gg/os-include/graphics/monitor.h
AQAction.o: /gg/os-include/graphics/displayinfo.h
AQAction.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQAction.o: /gg/os-include/hardware/custom.h
AQAction.o: /gg/os-include/graphics/rastport.h
AQAction.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQAction.o: /gg/os-include/devices/inputevent.h
AQAction.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQAction.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQAction.o: /gg/os-include/intuition/iobsolete.h
AQAction.o: /gg/os-include/intuition/preferences.h
AQString.o: AQString.h /gg/os-include/dos/dos.h AQKernel.h
AQString.o: /usr/include/string.h /gg/os-include/machine/ansi.h
AQString.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQWidget.o: ./AQWidget.h /gg/include/g++-3/vector.h
AQWidget.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQWidget.o: /gg/include/g++-3/stl_config.h
AQWidget.o: /gg/m68k-amigaos/include/_G_config.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQWidget.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQWidget.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQWidget.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQWidget.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQWidget.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQWidget.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQWidget.o: /gg/include/g++-3/stl_construct.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQWidget.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQWidget.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQWidget.o: /gg/os-include/machine/ansi.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQWidget.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQWidget.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQWidget.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQWidget.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQWidget.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQWidget.o: /gg/os-include/intuition/intuition.h
AQWidget.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQWidget.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQWidget.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQWidget.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQWidget.o: /gg/os-include/graphics/copper.h
AQWidget.o: /gg/os-include/graphics/gfxnodes.h
AQWidget.o: /gg/os-include/graphics/monitor.h
AQWidget.o: /gg/os-include/graphics/displayinfo.h
AQWidget.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQWidget.o: /gg/os-include/hardware/custom.h
AQWidget.o: /gg/os-include/graphics/rastport.h
AQWidget.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQWidget.o: /gg/os-include/devices/inputevent.h
AQWidget.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQWidget.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQWidget.o: /gg/os-include/intuition/iobsolete.h
AQWidget.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQWidget.o: AQString.h /gg/os-include/dos/dos.h
AQWidget.o: /gg/os-include/graphics/gfxmacros.h
AQWidget.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQWidget.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/graphics.h
AQWidget.o: /gg/os-include/graphics/scale.h
AQWidget.o: /gg/os-include/clib/graphics_protos.h
AQWidget.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQWidget.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQWidget.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/layers.h
AQWidget.o: /gg/os-include/clib/layers_protos.h
AQWidget.o: /gg/os-include/inline/layers.h /gg/os-include/proto/intuition.h
AQWidget.o: /gg/os-include/clib/intuition_protos.h
AQWidget.o: /gg/os-include/intuition/cghooks.h
AQWidget.o: /gg/os-include/inline/intuition.h AQLayout.h AQApplication.h
AQWidget.o: AQWindow.h
AQWindow.o: AQWindow.h /gg/os-include/exec/types.h AQKernel.h AQString.h
AQWindow.o: /gg/os-include/dos/dos.h AQLayout.h /gg/os-include/graphics/gfx.h
AQWindow.o: ./AQWidget.h /gg/include/g++-3/vector.h
AQWindow.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQWindow.o: /gg/include/g++-3/stl_config.h
AQWindow.o: /gg/m68k-amigaos/include/_G_config.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQWindow.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQWindow.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQWindow.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQWindow.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQWindow.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQWindow.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQWindow.o: /gg/include/g++-3/stl_construct.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQWindow.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQWindow.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQWindow.o: /gg/os-include/machine/ansi.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQWindow.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQWindow.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQWindow.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQWindow.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQWindow.o: /gg/include/g++-3/stl_vector.h
AQWindow.o: /gg/os-include/intuition/intuition.h
AQWindow.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQWindow.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQWindow.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQWindow.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQWindow.o: /gg/os-include/graphics/copper.h
AQWindow.o: /gg/os-include/graphics/gfxnodes.h
AQWindow.o: /gg/os-include/graphics/monitor.h
AQWindow.o: /gg/os-include/graphics/displayinfo.h
AQWindow.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQWindow.o: /gg/os-include/hardware/custom.h
AQWindow.o: /gg/os-include/graphics/rastport.h
AQWindow.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQWindow.o: /gg/os-include/devices/inputevent.h
AQWindow.o: /gg/os-include/devices/timer.h /gg/os-include/exec/io.h
AQWindow.o: /gg/os-include/intuition/screens.h
AQWindow.o: /gg/os-include/intuition/iobsolete.h
AQWindow.o: /gg/os-include/intuition/preferences.h ./AQObject.h
AQWindow.o: /usr/include/stdio.h /gg/os-include/libraries/gadtools.h
AQWindow.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQWindow.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/exec.h
AQWindow.o: /gg/os-include/exec/memory.h /gg/os-include/exec/devices.h
AQWindow.o: /gg/os-include/clib/exec_protos.h /gg/os-include/inline/exec.h
AQWindow.o: /gg/os-include/inline/macros.h /gg/os-include/inline/stubs.h
AQWindow.o: /gg/os-include/intuition/classes.h
AQWindow.o: /gg/os-include/intuition/classusr.h
AQWindow.o: /gg/os-include/libraries/commodities.h
AQWindow.o: /gg/os-include/proto/alib.h /gg/os-include/clib/alib_protos.h
AQWindow.o: /gg/os-include/devices/keymap.h
AQWindow.o: /gg/os-include/graphics/graphint.h /gg/os-include/inline/alib.h
AQWindow.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQWindow.o: /gg/os-include/clib/graphics_protos.h
AQWindow.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQWindow.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQWindow.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/intuition.h
AQWindow.o: /gg/os-include/clib/intuition_protos.h
AQWindow.o: /gg/os-include/intuition/cghooks.h
AQWindow.o: /gg/os-include/inline/intuition.h /gg/os-include/proto/gadtools.h
AQWindow.o: /gg/os-include/clib/gadtools_protos.h
AQWindow.o: /gg/os-include/inline/gadtools.h /gg/os-include/proto/layers.h
AQWindow.o: /gg/os-include/clib/layers_protos.h
AQWindow.o: /gg/os-include/inline/layers.h AQApplication.h
AQLabel.o: AQLabel.h ./AQWidget.h /gg/include/g++-3/vector.h
AQLabel.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQLabel.o: /gg/include/g++-3/stl_config.h
AQLabel.o: /gg/m68k-amigaos/include/_G_config.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQLabel.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQLabel.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQLabel.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQLabel.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQLabel.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQLabel.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQLabel.o: /gg/include/g++-3/stl_construct.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQLabel.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQLabel.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQLabel.o: /gg/os-include/machine/ansi.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQLabel.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQLabel.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQLabel.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQLabel.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQLabel.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQLabel.o: /gg/os-include/intuition/intuition.h
AQLabel.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQLabel.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQLabel.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQLabel.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQLabel.o: /gg/os-include/graphics/copper.h
AQLabel.o: /gg/os-include/graphics/gfxnodes.h
AQLabel.o: /gg/os-include/graphics/monitor.h
AQLabel.o: /gg/os-include/graphics/displayinfo.h
AQLabel.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQLabel.o: /gg/os-include/hardware/custom.h
AQLabel.o: /gg/os-include/graphics/rastport.h
AQLabel.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQLabel.o: /gg/os-include/devices/inputevent.h /gg/os-include/devices/timer.h
AQLabel.o: /gg/os-include/exec/types.h /gg/os-include/exec/io.h
AQLabel.o: /gg/os-include/intuition/screens.h
AQLabel.o: /gg/os-include/intuition/iobsolete.h
AQLabel.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQLabel.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h
AQLabel.o: /gg/os-include/graphics/gfxmacros.h
AQLabel.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQLabel.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/graphics.h
AQLabel.o: /gg/os-include/graphics/scale.h
AQLabel.o: /gg/os-include/clib/graphics_protos.h
AQLabel.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQLabel.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQLabel.o: /gg/os-include/inline/graphics.h
AQButton.o: AQButton.h ./AQWidget.h /gg/include/g++-3/vector.h
AQButton.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQButton.o: /gg/include/g++-3/stl_config.h
AQButton.o: /gg/m68k-amigaos/include/_G_config.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQButton.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQButton.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQButton.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQButton.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQButton.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQButton.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQButton.o: /gg/include/g++-3/stl_construct.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQButton.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQButton.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQButton.o: /gg/os-include/machine/ansi.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQButton.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQButton.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQButton.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQButton.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQButton.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQButton.o: /gg/os-include/intuition/intuition.h
AQButton.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQButton.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQButton.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQButton.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQButton.o: /gg/os-include/graphics/copper.h
AQButton.o: /gg/os-include/graphics/gfxnodes.h
AQButton.o: /gg/os-include/graphics/monitor.h
AQButton.o: /gg/os-include/graphics/displayinfo.h
AQButton.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQButton.o: /gg/os-include/hardware/custom.h
AQButton.o: /gg/os-include/graphics/rastport.h
AQButton.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQButton.o: /gg/os-include/devices/inputevent.h
AQButton.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQButton.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQButton.o: /gg/os-include/intuition/iobsolete.h
AQButton.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQButton.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h
AQButton.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQButton.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/graphics.h
AQButton.o: /gg/os-include/graphics/scale.h
AQButton.o: /gg/os-include/clib/graphics_protos.h
AQButton.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQButton.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQButton.o: /gg/os-include/inline/graphics.h
AQDialog.o: AQDialog.h ./AQWidget.h /gg/include/g++-3/vector.h
AQDialog.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQDialog.o: /gg/include/g++-3/stl_config.h
AQDialog.o: /gg/m68k-amigaos/include/_G_config.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQDialog.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQDialog.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQDialog.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQDialog.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQDialog.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQDialog.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQDialog.o: /gg/include/g++-3/stl_construct.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQDialog.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQDialog.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQDialog.o: /gg/os-include/machine/ansi.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQDialog.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQDialog.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQDialog.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQDialog.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQDialog.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQDialog.o: /gg/os-include/intuition/intuition.h
AQDialog.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQDialog.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQDialog.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQDialog.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQDialog.o: /gg/os-include/graphics/copper.h
AQDialog.o: /gg/os-include/graphics/gfxnodes.h
AQDialog.o: /gg/os-include/graphics/monitor.h
AQDialog.o: /gg/os-include/graphics/displayinfo.h
AQDialog.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQDialog.o: /gg/os-include/hardware/custom.h
AQDialog.o: /gg/os-include/graphics/rastport.h
AQDialog.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQDialog.o: /gg/os-include/devices/inputevent.h
AQDialog.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQDialog.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQDialog.o: /gg/os-include/intuition/iobsolete.h
AQDialog.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQDialog.o: AQString.h /gg/os-include/dos/dos.h /usr/include/stdio.h
AQDialog.o: /gg/os-include/exec/memory.h /gg/os-include/graphics/gfxbase.h
AQDialog.o: /gg/os-include/exec/libraries.h /gg/os-include/exec/interrupts.h
AQDialog.o: /gg/os-include/dos/exall.h /gg/os-include/proto/exec.h
AQDialog.o: /gg/os-include/exec/devices.h /gg/os-include/clib/exec_protos.h
AQDialog.o: /gg/os-include/inline/exec.h /gg/os-include/inline/macros.h
AQDialog.o: /gg/os-include/inline/stubs.h /gg/os-include/intuition/classes.h
AQDialog.o: /gg/os-include/intuition/classusr.h
AQDialog.o: /gg/os-include/libraries/commodities.h
AQDialog.o: /gg/os-include/proto/alib.h /gg/os-include/clib/alib_protos.h
AQDialog.o: /gg/os-include/devices/keymap.h
AQDialog.o: /gg/os-include/graphics/graphint.h /gg/os-include/inline/alib.h
AQDialog.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQDialog.o: /gg/os-include/clib/graphics_protos.h
AQDialog.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQDialog.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQDialog.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/dos.h
AQDialog.o: /gg/os-include/clib/dos_protos.h /gg/os-include/dos/dosextens.h
AQDialog.o: /gg/os-include/dos/record.h /gg/os-include/dos/rdargs.h
AQDialog.o: /gg/os-include/dos/dosasl.h /gg/os-include/dos/var.h
AQDialog.o: /gg/os-include/dos/notify.h /gg/os-include/dos/datetime.h
AQDialog.o: /gg/os-include/inline/dos.h AQLayout.h AQWindow.h AQButton.h
AQDialog.o: ./AQIcon.h ./AQTextDoc.h ./AQLineEdit.h ./AQTextEdit.h
AQDialog.o: ./AQListView.h ./AQScrollBar.h AQLabel.h ./AQSplitter.h
AQIcon.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQIcon.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/graphics.h
AQIcon.o: /gg/os-include/graphics/scale.h
AQIcon.o: /gg/os-include/clib/graphics_protos.h
AQIcon.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQIcon.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQIcon.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/layers.h
AQIcon.o: /gg/os-include/clib/layers_protos.h /gg/os-include/inline/layers.h
AQIcon.o: /gg/os-include/proto/icon.h /gg/os-include/clib/icon_protos.h
AQIcon.o: /gg/os-include/exec/types.h /gg/os-include/workbench/workbench.h
AQIcon.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQIcon.o: /gg/os-include/exec/tasks.h /gg/os-include/intuition/intuition.h
AQIcon.o: /gg/os-include/graphics/gfx.h /gg/os-include/graphics/clip.h
AQIcon.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/ports.h
AQIcon.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQIcon.o: /gg/os-include/graphics/copper.h /gg/os-include/graphics/gfxnodes.h
AQIcon.o: /gg/os-include/graphics/monitor.h
AQIcon.o: /gg/os-include/graphics/displayinfo.h
AQIcon.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQIcon.o: /gg/os-include/hardware/custom.h /gg/os-include/graphics/rastport.h
AQIcon.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQIcon.o: /gg/os-include/devices/inputevent.h /gg/os-include/devices/timer.h
AQIcon.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQIcon.o: /gg/os-include/intuition/iobsolete.h
AQIcon.o: /gg/os-include/intuition/preferences.h /gg/os-include/inline/icon.h
AQIcon.o: /gg/os-include/inline/macros.h /gg/os-include/inline/stubs.h
AQIcon.o: /gg/os-include/dos/dos.h /gg/os-include/intuition/classes.h
AQIcon.o: /gg/os-include/intuition/classusr.h
AQIcon.o: /gg/os-include/libraries/commodities.h AQString.h AQKernel.h
AQIcon.o: ./AQIcon.h
AQLayout.o: ./AQWidget.h /gg/include/g++-3/vector.h
AQLayout.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQLayout.o: /gg/include/g++-3/stl_config.h
AQLayout.o: /gg/m68k-amigaos/include/_G_config.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQLayout.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQLayout.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQLayout.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQLayout.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQLayout.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQLayout.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQLayout.o: /gg/include/g++-3/stl_construct.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQLayout.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQLayout.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQLayout.o: /gg/os-include/machine/ansi.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQLayout.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQLayout.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQLayout.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQLayout.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQLayout.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQLayout.o: /gg/os-include/intuition/intuition.h
AQLayout.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQLayout.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQLayout.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQLayout.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQLayout.o: /gg/os-include/graphics/copper.h
AQLayout.o: /gg/os-include/graphics/gfxnodes.h
AQLayout.o: /gg/os-include/graphics/monitor.h
AQLayout.o: /gg/os-include/graphics/displayinfo.h
AQLayout.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQLayout.o: /gg/os-include/hardware/custom.h
AQLayout.o: /gg/os-include/graphics/rastport.h
AQLayout.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQLayout.o: /gg/os-include/devices/inputevent.h
AQLayout.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQLayout.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQLayout.o: /gg/os-include/intuition/iobsolete.h
AQLayout.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQLayout.o: AQString.h /gg/os-include/dos/dos.h AQLayout.h
AQListView.o: ./AQListView.h ./AQWidget.h /gg/include/g++-3/vector.h
AQListView.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQListView.o: /gg/include/g++-3/stl_config.h
AQListView.o: /gg/m68k-amigaos/include/_G_config.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQListView.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQListView.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQListView.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQListView.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQListView.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQListView.o: /gg/include/g++-3/stl_iterator.h
AQListView.o: /gg/include/g++-3/type_traits.h
AQListView.o: /gg/include/g++-3/stl_construct.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQListView.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQListView.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQListView.o: /gg/os-include/machine/ansi.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQListView.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQListView.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQListView.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQListView.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQListView.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQListView.o: /gg/os-include/intuition/intuition.h
AQListView.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQListView.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQListView.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQListView.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQListView.o: /gg/os-include/graphics/copper.h
AQListView.o: /gg/os-include/graphics/gfxnodes.h
AQListView.o: /gg/os-include/graphics/monitor.h
AQListView.o: /gg/os-include/graphics/displayinfo.h
AQListView.o: /gg/os-include/graphics/modeid.h
AQListView.o: /gg/os-include/utility/tagitem.h
AQListView.o: /gg/os-include/hardware/custom.h
AQListView.o: /gg/os-include/graphics/rastport.h
AQListView.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQListView.o: /gg/os-include/devices/inputevent.h
AQListView.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQListView.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQListView.o: /gg/os-include/intuition/iobsolete.h
AQListView.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQListView.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h
AQListView.o: /gg/os-include/graphics/gfxbase.h
AQListView.o: /gg/os-include/exec/libraries.h
AQListView.o: /gg/os-include/exec/interrupts.h
AQListView.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQListView.o: /gg/os-include/clib/graphics_protos.h
AQListView.o: /gg/os-include/graphics/gels.h
AQListView.o: /gg/os-include/graphics/regions.h
AQListView.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQListView.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/layers.h
AQListView.o: /gg/os-include/clib/layers_protos.h
AQListView.o: /gg/os-include/inline/layers.h ./AQScrollBar.h
AQMenu.o: AQMenu.h ./AQWidget.h /gg/include/g++-3/vector.h
AQMenu.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQMenu.o: /gg/include/g++-3/stl_config.h /gg/m68k-amigaos/include/_G_config.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQMenu.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQMenu.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQMenu.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQMenu.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQMenu.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQMenu.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQMenu.o: /gg/include/g++-3/stl_construct.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQMenu.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQMenu.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQMenu.o: /gg/os-include/machine/ansi.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQMenu.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQMenu.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQMenu.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQMenu.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQMenu.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQMenu.o: /gg/os-include/intuition/intuition.h /gg/os-include/graphics/clip.h
AQMenu.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQMenu.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQMenu.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQMenu.o: /gg/os-include/graphics/view.h /gg/os-include/graphics/copper.h
AQMenu.o: /gg/os-include/graphics/gfxnodes.h
AQMenu.o: /gg/os-include/graphics/monitor.h
AQMenu.o: /gg/os-include/graphics/displayinfo.h
AQMenu.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQMenu.o: /gg/os-include/hardware/custom.h /gg/os-include/graphics/rastport.h
AQMenu.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQMenu.o: /gg/os-include/devices/inputevent.h /gg/os-include/devices/timer.h
AQMenu.o: /gg/os-include/exec/types.h /gg/os-include/exec/io.h
AQMenu.o: /gg/os-include/intuition/screens.h
AQMenu.o: /gg/os-include/intuition/iobsolete.h
AQMenu.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQMenu.o: AQString.h /gg/os-include/dos/dos.h
AQMenu.o: /gg/os-include/graphics/gfxmacros.h
AQMenu.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQMenu.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/graphics.h
AQMenu.o: /gg/os-include/graphics/scale.h
AQMenu.o: /gg/os-include/clib/graphics_protos.h
AQMenu.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQMenu.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQMenu.o: /gg/os-include/inline/graphics.h AQApplication.h
AQScrollBar.o: ./AQScrollBar.h ./AQWidget.h /gg/include/g++-3/vector.h
AQScrollBar.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQScrollBar.o: /gg/include/g++-3/stl_config.h
AQScrollBar.o: /gg/m68k-amigaos/include/_G_config.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQScrollBar.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQScrollBar.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQScrollBar.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQScrollBar.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQScrollBar.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQScrollBar.o: /gg/include/g++-3/stl_iterator.h
AQScrollBar.o: /gg/include/g++-3/type_traits.h
AQScrollBar.o: /gg/include/g++-3/stl_construct.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQScrollBar.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQScrollBar.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQScrollBar.o: /gg/os-include/machine/ansi.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQScrollBar.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQScrollBar.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQScrollBar.o: /gg/include/g++-3/stl_uninitialized.h
AQScrollBar.o: /gg/include/g++-3/alloc.h /gg/include/g++-3/stl_alloc.h
AQScrollBar.o: /gg/m68k-amigaos/include/assert.h
AQScrollBar.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQScrollBar.o: /gg/os-include/intuition/intuition.h
AQScrollBar.o: /gg/os-include/graphics/clip.h
AQScrollBar.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQScrollBar.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQScrollBar.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQScrollBar.o: /gg/os-include/graphics/view.h
AQScrollBar.o: /gg/os-include/graphics/copper.h
AQScrollBar.o: /gg/os-include/graphics/gfxnodes.h
AQScrollBar.o: /gg/os-include/graphics/monitor.h
AQScrollBar.o: /gg/os-include/graphics/displayinfo.h
AQScrollBar.o: /gg/os-include/graphics/modeid.h
AQScrollBar.o: /gg/os-include/utility/tagitem.h
AQScrollBar.o: /gg/os-include/hardware/custom.h
AQScrollBar.o: /gg/os-include/graphics/rastport.h
AQScrollBar.o: /gg/os-include/graphics/layers.h
AQScrollBar.o: /gg/os-include/graphics/text.h
AQScrollBar.o: /gg/os-include/devices/inputevent.h
AQScrollBar.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQScrollBar.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQScrollBar.o: /gg/os-include/intuition/iobsolete.h
AQScrollBar.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQScrollBar.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h
AQScrollBar.o: /gg/os-include/graphics/gfxbase.h
AQScrollBar.o: /gg/os-include/exec/libraries.h
AQScrollBar.o: /gg/os-include/exec/interrupts.h
AQScrollBar.o: /gg/os-include/proto/graphics.h
AQScrollBar.o: /gg/os-include/graphics/scale.h
AQScrollBar.o: /gg/os-include/clib/graphics_protos.h
AQScrollBar.o: /gg/os-include/graphics/gels.h
AQScrollBar.o: /gg/os-include/graphics/regions.h
AQScrollBar.o: /gg/os-include/graphics/sprite.h
AQScrollBar.o: /gg/os-include/hardware/blit.h
AQScrollBar.o: /gg/os-include/inline/graphics.h
AQSplitter.o: /gg/os-include/graphics/gfxbase.h
AQSplitter.o: /gg/os-include/exec/libraries.h
AQSplitter.o: /gg/os-include/exec/interrupts.h
AQSplitter.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQSplitter.o: /gg/os-include/clib/graphics_protos.h
AQSplitter.o: /gg/os-include/graphics/gels.h
AQSplitter.o: /gg/os-include/graphics/regions.h
AQSplitter.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQSplitter.o: /gg/os-include/inline/graphics.h AQLayout.h
AQSplitter.o: /gg/os-include/graphics/gfx.h AQKernel.h ./AQWidget.h
AQSplitter.o: /gg/include/g++-3/vector.h /gg/include/g++-3/algobase.h
AQSplitter.o: /gg/include/g++-3/pair.h /gg/include/g++-3/stl_config.h
AQSplitter.o: /gg/m68k-amigaos/include/_G_config.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQSplitter.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQSplitter.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQSplitter.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQSplitter.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQSplitter.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQSplitter.o: /gg/include/g++-3/stl_iterator.h
AQSplitter.o: /gg/include/g++-3/type_traits.h
AQSplitter.o: /gg/include/g++-3/stl_construct.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQSplitter.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQSplitter.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQSplitter.o: /gg/os-include/machine/ansi.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQSplitter.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQSplitter.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQSplitter.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQSplitter.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQSplitter.o: /gg/include/g++-3/stl_vector.h
AQSplitter.o: /gg/os-include/intuition/intuition.h
AQSplitter.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQSplitter.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQSplitter.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQSplitter.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQSplitter.o: /gg/os-include/graphics/copper.h
AQSplitter.o: /gg/os-include/graphics/gfxnodes.h
AQSplitter.o: /gg/os-include/graphics/monitor.h
AQSplitter.o: /gg/os-include/graphics/displayinfo.h
AQSplitter.o: /gg/os-include/graphics/modeid.h
AQSplitter.o: /gg/os-include/utility/tagitem.h
AQSplitter.o: /gg/os-include/hardware/custom.h
AQSplitter.o: /gg/os-include/graphics/rastport.h
AQSplitter.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQSplitter.o: /gg/os-include/devices/inputevent.h
AQSplitter.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQSplitter.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQSplitter.o: /gg/os-include/intuition/iobsolete.h
AQSplitter.o: /gg/os-include/intuition/preferences.h ./AQObject.h AQString.h
AQSplitter.o: /gg/os-include/dos/dos.h ./AQSplitter.h
AQTextEdit.o: ./AQTextEdit.h ./AQWidget.h /gg/include/g++-3/vector.h
AQTextEdit.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQTextEdit.o: /gg/include/g++-3/stl_config.h
AQTextEdit.o: /gg/m68k-amigaos/include/_G_config.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQTextEdit.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQTextEdit.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQTextEdit.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQTextEdit.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQTextEdit.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQTextEdit.o: /gg/include/g++-3/stl_iterator.h
AQTextEdit.o: /gg/include/g++-3/type_traits.h
AQTextEdit.o: /gg/include/g++-3/stl_construct.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQTextEdit.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQTextEdit.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQTextEdit.o: /gg/os-include/machine/ansi.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQTextEdit.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQTextEdit.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQTextEdit.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQTextEdit.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQTextEdit.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQTextEdit.o: /gg/os-include/intuition/intuition.h
AQTextEdit.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQTextEdit.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQTextEdit.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQTextEdit.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQTextEdit.o: /gg/os-include/graphics/copper.h
AQTextEdit.o: /gg/os-include/graphics/gfxnodes.h
AQTextEdit.o: /gg/os-include/graphics/monitor.h
AQTextEdit.o: /gg/os-include/graphics/displayinfo.h
AQTextEdit.o: /gg/os-include/graphics/modeid.h
AQTextEdit.o: /gg/os-include/utility/tagitem.h
AQTextEdit.o: /gg/os-include/hardware/custom.h
AQTextEdit.o: /gg/os-include/graphics/rastport.h
AQTextEdit.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQTextEdit.o: /gg/os-include/devices/inputevent.h
AQTextEdit.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQTextEdit.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQTextEdit.o: /gg/os-include/intuition/iobsolete.h
AQTextEdit.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQTextEdit.o: AQString.h /gg/os-include/dos/dos.h
AQTextEdit.o: /gg/os-include/graphics/gfxbase.h
AQTextEdit.o: /gg/os-include/exec/libraries.h
AQTextEdit.o: /gg/os-include/exec/interrupts.h
AQTextEdit.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQTextEdit.o: /gg/os-include/clib/graphics_protos.h
AQTextEdit.o: /gg/os-include/graphics/gels.h
AQTextEdit.o: /gg/os-include/graphics/regions.h
AQTextEdit.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQTextEdit.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/layers.h
AQTextEdit.o: /gg/os-include/clib/layers_protos.h
AQTextEdit.o: /gg/os-include/inline/layers.h ./AQTextDoc.h ./AQScrollBar.h
AQTextEdit.o: ./AQIcon.h AQMenu.h AQApplication.h ./AQClipboard.h AQAction.h
AQTextEdit.o: /usr/include/stdio.h
AQTextDoc.o: ./AQTextDoc.h AQKernel.h AQString.h /gg/os-include/dos/dos.h
AQTextDoc.o: /usr/include/string.h /gg/os-include/machine/ansi.h
AQTextDoc.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQTextDoc.o: /gg/os-include/graphics/gfxbase.h
AQTextDoc.o: /gg/os-include/exec/libraries.h /gg/os-include/exec/interrupts.h
AQTextDoc.o: /gg/os-include/graphics/rastport.h
AQTextDoc.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQTextDoc.o: /gg/os-include/clib/graphics_protos.h
AQTextDoc.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQTextDoc.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQTextDoc.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/dos.h
AQTextDoc.o: /gg/os-include/clib/dos_protos.h /gg/os-include/dos/dosextens.h
AQTextDoc.o: /gg/os-include/dos/record.h /gg/os-include/dos/rdargs.h
AQTextDoc.o: /gg/os-include/dos/dosasl.h /gg/os-include/dos/var.h
AQTextDoc.o: /gg/os-include/dos/notify.h /gg/os-include/dos/datetime.h
AQTextDoc.o: /gg/os-include/inline/dos.h
AQLineEdit.o: ./AQLineEdit.h ./AQTextEdit.h ./AQWidget.h
AQLineEdit.o: /gg/include/g++-3/vector.h /gg/include/g++-3/algobase.h
AQLineEdit.o: /gg/include/g++-3/pair.h /gg/include/g++-3/stl_config.h
AQLineEdit.o: /gg/m68k-amigaos/include/_G_config.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQLineEdit.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQLineEdit.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQLineEdit.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQLineEdit.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQLineEdit.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQLineEdit.o: /gg/include/g++-3/stl_iterator.h
AQLineEdit.o: /gg/include/g++-3/type_traits.h
AQLineEdit.o: /gg/include/g++-3/stl_construct.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQLineEdit.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQLineEdit.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQLineEdit.o: /gg/os-include/machine/ansi.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQLineEdit.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQLineEdit.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQLineEdit.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQLineEdit.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQLineEdit.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQLineEdit.o: /gg/os-include/intuition/intuition.h
AQLineEdit.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQLineEdit.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQLineEdit.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQLineEdit.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQLineEdit.o: /gg/os-include/graphics/copper.h
AQLineEdit.o: /gg/os-include/graphics/gfxnodes.h
AQLineEdit.o: /gg/os-include/graphics/monitor.h
AQLineEdit.o: /gg/os-include/graphics/displayinfo.h
AQLineEdit.o: /gg/os-include/graphics/modeid.h
AQLineEdit.o: /gg/os-include/utility/tagitem.h
AQLineEdit.o: /gg/os-include/hardware/custom.h
AQLineEdit.o: /gg/os-include/graphics/rastport.h
AQLineEdit.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQLineEdit.o: /gg/os-include/devices/inputevent.h
AQLineEdit.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQLineEdit.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQLineEdit.o: /gg/os-include/intuition/iobsolete.h
AQLineEdit.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQLineEdit.o: AQString.h /gg/os-include/dos/dos.h /usr/include/stdio.h
AQMainWindow.o: AQMainWindow.h ./AQWidget.h /gg/include/g++-3/vector.h
AQMainWindow.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQMainWindow.o: /gg/include/g++-3/stl_config.h
AQMainWindow.o: /gg/m68k-amigaos/include/_G_config.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQMainWindow.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQMainWindow.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQMainWindow.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQMainWindow.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQMainWindow.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQMainWindow.o: /gg/include/g++-3/stl_iterator.h
AQMainWindow.o: /gg/include/g++-3/type_traits.h
AQMainWindow.o: /gg/include/g++-3/stl_construct.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQMainWindow.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQMainWindow.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQMainWindow.o: /gg/os-include/machine/ansi.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQMainWindow.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQMainWindow.o: /gg/os-include/machine/types.h
AQMainWindow.o: /gg/os-include/machine/endian.h
AQMainWindow.o: /gg/include/g++-3/stl_uninitialized.h
AQMainWindow.o: /gg/include/g++-3/alloc.h /gg/include/g++-3/stl_alloc.h
AQMainWindow.o: /gg/m68k-amigaos/include/assert.h
AQMainWindow.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQMainWindow.o: /gg/os-include/intuition/intuition.h
AQMainWindow.o: /gg/os-include/graphics/clip.h
AQMainWindow.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQMainWindow.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQMainWindow.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQMainWindow.o: /gg/os-include/graphics/view.h
AQMainWindow.o: /gg/os-include/graphics/copper.h
AQMainWindow.o: /gg/os-include/graphics/gfxnodes.h
AQMainWindow.o: /gg/os-include/graphics/monitor.h
AQMainWindow.o: /gg/os-include/graphics/displayinfo.h
AQMainWindow.o: /gg/os-include/graphics/modeid.h
AQMainWindow.o: /gg/os-include/utility/tagitem.h
AQMainWindow.o: /gg/os-include/hardware/custom.h
AQMainWindow.o: /gg/os-include/graphics/rastport.h
AQMainWindow.o: /gg/os-include/graphics/layers.h
AQMainWindow.o: /gg/os-include/graphics/text.h
AQMainWindow.o: /gg/os-include/devices/inputevent.h
AQMainWindow.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQMainWindow.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQMainWindow.o: /gg/os-include/intuition/iobsolete.h
AQMainWindow.o: /gg/os-include/intuition/preferences.h AQKernel.h
AQMainWindow.o: ./AQObject.h AQString.h /gg/os-include/dos/dos.h
AQMainWindow.o: /gg/os-include/graphics/gfxbase.h
AQMainWindow.o: /gg/os-include/exec/libraries.h
AQMainWindow.o: /gg/os-include/exec/interrupts.h
AQMainWindow.o: /gg/os-include/proto/graphics.h
AQMainWindow.o: /gg/os-include/graphics/scale.h
AQMainWindow.o: /gg/os-include/clib/graphics_protos.h
AQMainWindow.o: /gg/os-include/graphics/gels.h
AQMainWindow.o: /gg/os-include/graphics/regions.h
AQMainWindow.o: /gg/os-include/graphics/sprite.h
AQMainWindow.o: /gg/os-include/hardware/blit.h
AQMainWindow.o: /gg/os-include/inline/graphics.h
DevStudio.o: ./AQScrollBar.h ./AQWidget.h /gg/include/g++-3/vector.h
DevStudio.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
DevStudio.o: /gg/include/g++-3/stl_config.h
DevStudio.o: /gg/m68k-amigaos/include/_G_config.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
DevStudio.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
DevStudio.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
DevStudio.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
DevStudio.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
DevStudio.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
DevStudio.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
DevStudio.o: /gg/include/g++-3/stl_construct.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
DevStudio.o: /gg/include/g++-3/stl_raw_storage_iter.h
DevStudio.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
DevStudio.o: /gg/os-include/machine/ansi.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
DevStudio.o: /usr/include/stdlib.h /usr/include/sys/types.h
DevStudio.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
DevStudio.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
DevStudio.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
DevStudio.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
DevStudio.o: /gg/os-include/intuition/intuition.h
DevStudio.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
DevStudio.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
DevStudio.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
DevStudio.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
DevStudio.o: /gg/os-include/graphics/copper.h
DevStudio.o: /gg/os-include/graphics/gfxnodes.h
DevStudio.o: /gg/os-include/graphics/monitor.h
DevStudio.o: /gg/os-include/graphics/displayinfo.h
DevStudio.o: /gg/os-include/graphics/modeid.h
DevStudio.o: /gg/os-include/utility/tagitem.h
DevStudio.o: /gg/os-include/hardware/custom.h
DevStudio.o: /gg/os-include/graphics/rastport.h
DevStudio.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
DevStudio.o: /gg/os-include/devices/inputevent.h
DevStudio.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
DevStudio.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
DevStudio.o: /gg/os-include/intuition/iobsolete.h
DevStudio.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
DevStudio.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h AQDialog.h
DevStudio.o: ./AQTextEdit.h ./AQTextDoc.h AQLayout.h ./AQListView.h
DevStudio.o: DevStudio.h AQMainWindow.h
main.o: /usr/include/stdio.h /usr/include/sys/types.h
main.o: /gg/os-include/machine/types.h /usr/include/sys/cdefs.h
main.o: /gg/os-include/machine/cdefs.h /gg/os-include/machine/ansi.h
main.o: /gg/os-include/machine/endian.h /usr/include/stdlib.h AQApplication.h
main.o: /gg/include/g++-3/vector.h /gg/include/g++-3/algobase.h
main.o: /gg/include/g++-3/pair.h /gg/include/g++-3/stl_config.h
main.o: /gg/m68k-amigaos/include/_G_config.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
main.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
main.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
main.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
main.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
main.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
main.o: /gg/include/g++-3/stl_construct.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
main.o: /gg/include/g++-3/stl_raw_storage_iter.h
main.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
main.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
main.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
main.o: /gg/include/g++-3/stl_vector.h AQKernel.h DevStudio.h AQMainWindow.h
main.o: ./AQWidget.h /gg/os-include/graphics/gfx.h
main.o: /gg/os-include/intuition/intuition.h /gg/os-include/graphics/clip.h
main.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
main.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
main.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
main.o: /gg/os-include/graphics/view.h /gg/os-include/graphics/copper.h
main.o: /gg/os-include/graphics/gfxnodes.h /gg/os-include/graphics/monitor.h
main.o: /gg/os-include/graphics/displayinfo.h
main.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
main.o: /gg/os-include/hardware/custom.h /gg/os-include/graphics/rastport.h
main.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
main.o: /gg/os-include/devices/inputevent.h /gg/os-include/devices/timer.h
main.o: /gg/os-include/exec/types.h /gg/os-include/exec/io.h
main.o: /gg/os-include/intuition/screens.h
main.o: /gg/os-include/intuition/iobsolete.h
main.o: /gg/os-include/intuition/preferences.h ./AQObject.h AQString.h
main.o: /gg/os-include/dos/dos.h
# DO NOT DELETE

AQApplication.o: AQApplication.h /gg/include/g++-3/vector.h
AQApplication.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQApplication.o: /gg/include/g++-3/stl_config.h
AQApplication.o: /gg/m68k-amigaos/include/_G_config.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQApplication.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQApplication.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQApplication.o: /gg/include/g++-3/stl_function.h
AQApplication.o: /gg/include/g++-3/iostream.h /gg/include/g++-3/streambuf.h
AQApplication.o: /gg/include/g++-3/libio.h /usr/include/sys/cdefs.h
AQApplication.o: /gg/os-include/machine/cdefs.h
AQApplication.o: /gg/include/g++-3/stl_iterator.h
AQApplication.o: /gg/include/g++-3/type_traits.h
AQApplication.o: /gg/include/g++-3/stl_construct.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQApplication.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQApplication.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQApplication.o: /gg/os-include/machine/ansi.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQApplication.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQApplication.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQApplication.o: /gg/os-include/machine/types.h
AQApplication.o: /gg/os-include/machine/endian.h
AQApplication.o: /gg/include/g++-3/stl_uninitialized.h
AQApplication.o: /gg/include/g++-3/alloc.h /gg/include/g++-3/stl_alloc.h
AQApplication.o: /gg/m68k-amigaos/include/assert.h
AQApplication.o: /gg/include/g++-3/stl_vector.h AQKernel.h
AQApplication.o: /usr/include/stdio.h /gg/include/g++-3/algorithm
AQApplication.o: /gg/include/g++-3/stl_tempbuf.h /gg/include/g++-3/stl_algo.h
AQApplication.o: /gg/include/g++-3/stl_heap.h /gg/os-include/exec/types.h
AQApplication.o: /gg/os-include/intuition/intuition.h
AQApplication.o: /gg/os-include/graphics/gfx.h /gg/os-include/graphics/clip.h
AQApplication.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQApplication.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQApplication.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQApplication.o: /gg/os-include/graphics/view.h
AQApplication.o: /gg/os-include/graphics/copper.h
AQApplication.o: /gg/os-include/graphics/gfxnodes.h
AQApplication.o: /gg/os-include/graphics/monitor.h
AQApplication.o: /gg/os-include/graphics/displayinfo.h
AQApplication.o: /gg/os-include/graphics/modeid.h
AQApplication.o: /gg/os-include/utility/tagitem.h
AQApplication.o: /gg/os-include/hardware/custom.h
AQApplication.o: /gg/os-include/graphics/rastport.h
AQApplication.o: /gg/os-include/graphics/layers.h
AQApplication.o: /gg/os-include/graphics/text.h
AQApplication.o: /gg/os-include/devices/inputevent.h
AQApplication.o: /gg/os-include/devices/timer.h /gg/os-include/exec/io.h
AQApplication.o: /gg/os-include/intuition/screens.h
AQApplication.o: /gg/os-include/intuition/iobsolete.h
AQApplication.o: /gg/os-include/intuition/preferences.h
AQApplication.o: /gg/os-include/libraries/gadtools.h
AQApplication.o: /gg/os-include/proto/exec.h /gg/os-include/exec/memory.h
AQApplication.o: /gg/os-include/exec/devices.h
AQApplication.o: /gg/os-include/exec/libraries.h
AQApplication.o: /gg/os-include/clib/exec_protos.h
AQApplication.o: /gg/os-include/inline/exec.h /gg/os-include/inline/macros.h
AQApplication.o: /gg/os-include/inline/stubs.h /gg/os-include/dos/dos.h
AQApplication.o: /gg/os-include/intuition/classes.h
AQApplication.o: /gg/os-include/intuition/classusr.h
AQApplication.o: /gg/os-include/libraries/commodities.h
AQApplication.o: /gg/os-include/proto/alib.h
AQApplication.o: /gg/os-include/clib/alib_protos.h
AQApplication.o: /gg/os-include/devices/keymap.h
AQApplication.o: /gg/os-include/graphics/graphint.h
AQApplication.o: /gg/os-include/inline/alib.h /gg/os-include/proto/graphics.h
AQApplication.o: /gg/os-include/graphics/scale.h
AQApplication.o: /gg/os-include/clib/graphics_protos.h
AQApplication.o: /gg/os-include/graphics/gels.h
AQApplication.o: /gg/os-include/graphics/regions.h
AQApplication.o: /gg/os-include/graphics/sprite.h
AQApplication.o: /gg/os-include/hardware/blit.h
AQApplication.o: /gg/os-include/inline/graphics.h
AQApplication.o: /gg/os-include/proto/intuition.h
AQApplication.o: /gg/os-include/clib/intuition_protos.h
AQApplication.o: /gg/os-include/intuition/cghooks.h
AQApplication.o: /gg/os-include/inline/intuition.h
AQApplication.o: /gg/os-include/proto/gadtools.h
AQApplication.o: /gg/os-include/clib/gadtools_protos.h
AQApplication.o: /gg/os-include/inline/gadtools.h
AQApplication.o: /gg/os-include/proto/layers.h
AQApplication.o: /gg/os-include/clib/layers_protos.h
AQApplication.o: /gg/os-include/inline/layers.h ./AQWindow.h AQString.h
AQApplication.o: AQLayout.h ./AQWidget.h ./AQObject.h ./AQClipboard.h
AQApplication.o: ./AQAction.h
AQClipboard.o: AQString.h /gg/os-include/dos/dos.h AQKernel.h ./AQClipboard.h
AQAction.o: /gg/os-include/intuition/intuition.h
AQAction.o: /gg/os-include/graphics/gfx.h /gg/os-include/graphics/clip.h
AQAction.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQAction.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQAction.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQAction.o: /gg/os-include/graphics/view.h /gg/os-include/graphics/copper.h
AQAction.o: /gg/os-include/graphics/gfxnodes.h
AQAction.o: /gg/os-include/graphics/monitor.h
AQAction.o: /gg/os-include/graphics/displayinfo.h
AQAction.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQAction.o: /gg/os-include/hardware/custom.h
AQAction.o: /gg/os-include/graphics/rastport.h
AQAction.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQAction.o: /gg/os-include/devices/inputevent.h
AQAction.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQAction.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQAction.o: /gg/os-include/intuition/iobsolete.h
AQAction.o: /gg/os-include/intuition/preferences.h /usr/include/ctype.h
AQAction.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQAction.o: ./AQWidget.h /gg/include/g++-3/vector.h
AQAction.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQAction.o: /gg/include/g++-3/stl_config.h
AQAction.o: /gg/m68k-amigaos/include/_G_config.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQAction.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQAction.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQAction.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQAction.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQAction.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQAction.o: /gg/include/g++-3/stl_construct.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQAction.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQAction.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQAction.o: /gg/os-include/machine/ansi.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQAction.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQAction.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQAction.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQAction.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQAction.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQAction.o: /gg/include/g++-3/stl_vector.h AQKernel.h ./AQObject.h AQString.h
AQAction.o: /gg/os-include/dos/dos.h ./AQAction.h
AQString.o: AQString.h /gg/os-include/dos/dos.h AQKernel.h
AQString.o: /usr/include/string.h /gg/os-include/machine/ansi.h
AQString.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQWidget.o: ./AQWidget.h /gg/include/g++-3/vector.h
AQWidget.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQWidget.o: /gg/include/g++-3/stl_config.h
AQWidget.o: /gg/m68k-amigaos/include/_G_config.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQWidget.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQWidget.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQWidget.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQWidget.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQWidget.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQWidget.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQWidget.o: /gg/include/g++-3/stl_construct.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQWidget.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQWidget.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQWidget.o: /gg/os-include/machine/ansi.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQWidget.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQWidget.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQWidget.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQWidget.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQWidget.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQWidget.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQWidget.o: /gg/os-include/intuition/intuition.h
AQWidget.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQWidget.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQWidget.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQWidget.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQWidget.o: /gg/os-include/graphics/copper.h
AQWidget.o: /gg/os-include/graphics/gfxnodes.h
AQWidget.o: /gg/os-include/graphics/monitor.h
AQWidget.o: /gg/os-include/graphics/displayinfo.h
AQWidget.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQWidget.o: /gg/os-include/hardware/custom.h
AQWidget.o: /gg/os-include/graphics/rastport.h
AQWidget.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQWidget.o: /gg/os-include/devices/inputevent.h
AQWidget.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQWidget.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQWidget.o: /gg/os-include/intuition/iobsolete.h
AQWidget.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQWidget.o: AQString.h /gg/os-include/dos/dos.h /gg/include/g++-3/algorithm
AQWidget.o: /gg/include/g++-3/stl_tempbuf.h /gg/include/g++-3/stl_algo.h
AQWidget.o: /gg/include/g++-3/stl_heap.h /gg/os-include/graphics/gfxbase.h
AQWidget.o: /gg/os-include/exec/libraries.h /gg/os-include/exec/interrupts.h
AQWidget.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQWidget.o: /gg/os-include/clib/graphics_protos.h
AQWidget.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQWidget.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQWidget.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/layers.h
AQWidget.o: /gg/os-include/clib/layers_protos.h
AQWidget.o: /gg/os-include/inline/layers.h /gg/os-include/proto/intuition.h
AQWidget.o: /gg/os-include/clib/intuition_protos.h
AQWidget.o: /gg/os-include/intuition/cghooks.h
AQWidget.o: /gg/os-include/inline/intuition.h AQLayout.h AQApplication.h
AQWidget.o: ./AQWindow.h ./AQAction.h
AQWindow.o: ./AQWindow.h /gg/os-include/exec/types.h AQKernel.h AQString.h
AQWindow.o: /gg/os-include/dos/dos.h AQLayout.h /gg/os-include/graphics/gfx.h
AQWindow.o: ./AQWidget.h /gg/include/g++-3/vector.h
AQWindow.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQWindow.o: /gg/include/g++-3/stl_config.h
AQWindow.o: /gg/m68k-amigaos/include/_G_config.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQWindow.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQWindow.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQWindow.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQWindow.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQWindow.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQWindow.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQWindow.o: /gg/include/g++-3/stl_construct.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQWindow.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQWindow.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQWindow.o: /gg/os-include/machine/ansi.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQWindow.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQWindow.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQWindow.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQWindow.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQWindow.o: /gg/include/g++-3/stl_vector.h
AQWindow.o: /gg/os-include/intuition/intuition.h
AQWindow.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQWindow.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQWindow.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQWindow.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQWindow.o: /gg/os-include/graphics/copper.h
AQWindow.o: /gg/os-include/graphics/gfxnodes.h
AQWindow.o: /gg/os-include/graphics/monitor.h
AQWindow.o: /gg/os-include/graphics/displayinfo.h
AQWindow.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQWindow.o: /gg/os-include/hardware/custom.h
AQWindow.o: /gg/os-include/graphics/rastport.h
AQWindow.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQWindow.o: /gg/os-include/devices/inputevent.h
AQWindow.o: /gg/os-include/devices/timer.h /gg/os-include/exec/io.h
AQWindow.o: /gg/os-include/intuition/screens.h
AQWindow.o: /gg/os-include/intuition/iobsolete.h
AQWindow.o: /gg/os-include/intuition/preferences.h ./AQObject.h
AQWindow.o: /usr/include/stdio.h /gg/os-include/libraries/gadtools.h
AQWindow.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQWindow.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/exec.h
AQWindow.o: /gg/os-include/exec/memory.h /gg/os-include/exec/devices.h
AQWindow.o: /gg/os-include/clib/exec_protos.h /gg/os-include/inline/exec.h
AQWindow.o: /gg/os-include/inline/macros.h /gg/os-include/inline/stubs.h
AQWindow.o: /gg/os-include/intuition/classes.h
AQWindow.o: /gg/os-include/intuition/classusr.h
AQWindow.o: /gg/os-include/libraries/commodities.h
AQWindow.o: /gg/os-include/proto/alib.h /gg/os-include/clib/alib_protos.h
AQWindow.o: /gg/os-include/devices/keymap.h
AQWindow.o: /gg/os-include/graphics/graphint.h /gg/os-include/inline/alib.h
AQWindow.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQWindow.o: /gg/os-include/clib/graphics_protos.h
AQWindow.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQWindow.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQWindow.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/intuition.h
AQWindow.o: /gg/os-include/clib/intuition_protos.h
AQWindow.o: /gg/os-include/intuition/cghooks.h
AQWindow.o: /gg/os-include/inline/intuition.h /gg/os-include/proto/gadtools.h
AQWindow.o: /gg/os-include/clib/gadtools_protos.h
AQWindow.o: /gg/os-include/inline/gadtools.h /gg/os-include/proto/layers.h
AQWindow.o: /gg/os-include/clib/layers_protos.h
AQWindow.o: /gg/os-include/inline/layers.h AQApplication.h
AQLabel.o: AQLabel.h ./AQWidget.h /gg/include/g++-3/vector.h
AQLabel.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQLabel.o: /gg/include/g++-3/stl_config.h
AQLabel.o: /gg/m68k-amigaos/include/_G_config.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQLabel.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQLabel.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQLabel.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQLabel.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQLabel.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQLabel.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQLabel.o: /gg/include/g++-3/stl_construct.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQLabel.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQLabel.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQLabel.o: /gg/os-include/machine/ansi.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQLabel.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQLabel.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQLabel.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQLabel.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQLabel.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQLabel.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQLabel.o: /gg/os-include/intuition/intuition.h
AQLabel.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQLabel.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQLabel.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQLabel.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQLabel.o: /gg/os-include/graphics/copper.h
AQLabel.o: /gg/os-include/graphics/gfxnodes.h
AQLabel.o: /gg/os-include/graphics/monitor.h
AQLabel.o: /gg/os-include/graphics/displayinfo.h
AQLabel.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQLabel.o: /gg/os-include/hardware/custom.h
AQLabel.o: /gg/os-include/graphics/rastport.h
AQLabel.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQLabel.o: /gg/os-include/devices/inputevent.h /gg/os-include/devices/timer.h
AQLabel.o: /gg/os-include/exec/types.h /gg/os-include/exec/io.h
AQLabel.o: /gg/os-include/intuition/screens.h
AQLabel.o: /gg/os-include/intuition/iobsolete.h
AQLabel.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQLabel.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h
AQLabel.o: /gg/os-include/graphics/gfxmacros.h
AQLabel.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQLabel.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/graphics.h
AQLabel.o: /gg/os-include/graphics/scale.h
AQLabel.o: /gg/os-include/clib/graphics_protos.h
AQLabel.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQLabel.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQLabel.o: /gg/os-include/inline/graphics.h
AQButton.o: AQButton.h ./AQWidget.h /gg/include/g++-3/vector.h
AQButton.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQButton.o: /gg/include/g++-3/stl_config.h
AQButton.o: /gg/m68k-amigaos/include/_G_config.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQButton.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQButton.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQButton.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQButton.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQButton.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQButton.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQButton.o: /gg/include/g++-3/stl_construct.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQButton.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQButton.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQButton.o: /gg/os-include/machine/ansi.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQButton.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQButton.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQButton.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQButton.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQButton.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQButton.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQButton.o: /gg/os-include/intuition/intuition.h
AQButton.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQButton.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQButton.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQButton.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQButton.o: /gg/os-include/graphics/copper.h
AQButton.o: /gg/os-include/graphics/gfxnodes.h
AQButton.o: /gg/os-include/graphics/monitor.h
AQButton.o: /gg/os-include/graphics/displayinfo.h
AQButton.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQButton.o: /gg/os-include/hardware/custom.h
AQButton.o: /gg/os-include/graphics/rastport.h
AQButton.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQButton.o: /gg/os-include/devices/inputevent.h
AQButton.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQButton.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQButton.o: /gg/os-include/intuition/iobsolete.h
AQButton.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQButton.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h
AQButton.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQButton.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/graphics.h
AQButton.o: /gg/os-include/graphics/scale.h
AQButton.o: /gg/os-include/clib/graphics_protos.h
AQButton.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQButton.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQButton.o: /gg/os-include/inline/graphics.h
AQDialog.o: AQDialog.h ./AQWidget.h /gg/include/g++-3/vector.h
AQDialog.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQDialog.o: /gg/include/g++-3/stl_config.h
AQDialog.o: /gg/m68k-amigaos/include/_G_config.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQDialog.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQDialog.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQDialog.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQDialog.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQDialog.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQDialog.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQDialog.o: /gg/include/g++-3/stl_construct.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQDialog.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQDialog.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQDialog.o: /gg/os-include/machine/ansi.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQDialog.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQDialog.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQDialog.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQDialog.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQDialog.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQDialog.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQDialog.o: /gg/os-include/intuition/intuition.h
AQDialog.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQDialog.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQDialog.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQDialog.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQDialog.o: /gg/os-include/graphics/copper.h
AQDialog.o: /gg/os-include/graphics/gfxnodes.h
AQDialog.o: /gg/os-include/graphics/monitor.h
AQDialog.o: /gg/os-include/graphics/displayinfo.h
AQDialog.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQDialog.o: /gg/os-include/hardware/custom.h
AQDialog.o: /gg/os-include/graphics/rastport.h
AQDialog.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQDialog.o: /gg/os-include/devices/inputevent.h
AQDialog.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQDialog.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQDialog.o: /gg/os-include/intuition/iobsolete.h
AQDialog.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQDialog.o: AQString.h /gg/os-include/dos/dos.h /usr/include/stdio.h
AQDialog.o: /gg/os-include/exec/memory.h /gg/os-include/graphics/gfxbase.h
AQDialog.o: /gg/os-include/exec/libraries.h /gg/os-include/exec/interrupts.h
AQDialog.o: /gg/os-include/dos/exall.h /gg/os-include/proto/exec.h
AQDialog.o: /gg/os-include/exec/devices.h /gg/os-include/clib/exec_protos.h
AQDialog.o: /gg/os-include/inline/exec.h /gg/os-include/inline/macros.h
AQDialog.o: /gg/os-include/inline/stubs.h /gg/os-include/intuition/classes.h
AQDialog.o: /gg/os-include/intuition/classusr.h
AQDialog.o: /gg/os-include/libraries/commodities.h
AQDialog.o: /gg/os-include/proto/alib.h /gg/os-include/clib/alib_protos.h
AQDialog.o: /gg/os-include/devices/keymap.h
AQDialog.o: /gg/os-include/graphics/graphint.h /gg/os-include/inline/alib.h
AQDialog.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQDialog.o: /gg/os-include/clib/graphics_protos.h
AQDialog.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQDialog.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQDialog.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/dos.h
AQDialog.o: /gg/os-include/clib/dos_protos.h /gg/os-include/dos/dosextens.h
AQDialog.o: /gg/os-include/dos/record.h /gg/os-include/dos/rdargs.h
AQDialog.o: /gg/os-include/dos/dosasl.h /gg/os-include/dos/var.h
AQDialog.o: /gg/os-include/dos/notify.h /gg/os-include/dos/datetime.h
AQDialog.o: /gg/os-include/inline/dos.h AQLayout.h ./AQWindow.h AQButton.h
AQDialog.o: ./AQIcon.h ./AQTextDoc.h ./AQLineEdit.h ./AQTextEdit.h
AQDialog.o: ./AQListView.h ./AQScrollBar.h AQLabel.h ./AQSplitter.h
AQDialog.o: AQApplication.h
AQIcon.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQIcon.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/graphics.h
AQIcon.o: /gg/os-include/graphics/scale.h
AQIcon.o: /gg/os-include/clib/graphics_protos.h
AQIcon.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQIcon.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQIcon.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/layers.h
AQIcon.o: /gg/os-include/clib/layers_protos.h /gg/os-include/inline/layers.h
AQIcon.o: /gg/os-include/proto/icon.h /gg/os-include/clib/icon_protos.h
AQIcon.o: /gg/os-include/exec/types.h /gg/os-include/workbench/workbench.h
AQIcon.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQIcon.o: /gg/os-include/exec/tasks.h /gg/os-include/intuition/intuition.h
AQIcon.o: /gg/os-include/graphics/gfx.h /gg/os-include/graphics/clip.h
AQIcon.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/ports.h
AQIcon.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQIcon.o: /gg/os-include/graphics/copper.h /gg/os-include/graphics/gfxnodes.h
AQIcon.o: /gg/os-include/graphics/monitor.h
AQIcon.o: /gg/os-include/graphics/displayinfo.h
AQIcon.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQIcon.o: /gg/os-include/hardware/custom.h /gg/os-include/graphics/rastport.h
AQIcon.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQIcon.o: /gg/os-include/devices/inputevent.h /gg/os-include/devices/timer.h
AQIcon.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQIcon.o: /gg/os-include/intuition/iobsolete.h
AQIcon.o: /gg/os-include/intuition/preferences.h /gg/os-include/inline/icon.h
AQIcon.o: /gg/os-include/inline/macros.h /gg/os-include/inline/stubs.h
AQIcon.o: /gg/os-include/dos/dos.h /gg/os-include/intuition/classes.h
AQIcon.o: /gg/os-include/intuition/classusr.h
AQIcon.o: /gg/os-include/libraries/commodities.h AQString.h AQKernel.h
AQIcon.o: ./AQIcon.h
AQLayout.o: ./AQWidget.h /gg/include/g++-3/vector.h
AQLayout.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQLayout.o: /gg/include/g++-3/stl_config.h
AQLayout.o: /gg/m68k-amigaos/include/_G_config.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQLayout.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQLayout.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQLayout.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQLayout.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQLayout.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQLayout.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQLayout.o: /gg/include/g++-3/stl_construct.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQLayout.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQLayout.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQLayout.o: /gg/os-include/machine/ansi.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQLayout.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQLayout.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQLayout.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQLayout.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQLayout.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQLayout.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQLayout.o: /gg/os-include/intuition/intuition.h
AQLayout.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQLayout.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQLayout.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQLayout.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQLayout.o: /gg/os-include/graphics/copper.h
AQLayout.o: /gg/os-include/graphics/gfxnodes.h
AQLayout.o: /gg/os-include/graphics/monitor.h
AQLayout.o: /gg/os-include/graphics/displayinfo.h
AQLayout.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQLayout.o: /gg/os-include/hardware/custom.h
AQLayout.o: /gg/os-include/graphics/rastport.h
AQLayout.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQLayout.o: /gg/os-include/devices/inputevent.h
AQLayout.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQLayout.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQLayout.o: /gg/os-include/intuition/iobsolete.h
AQLayout.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQLayout.o: AQString.h /gg/os-include/dos/dos.h AQLayout.h
AQListView.o: ./AQListView.h ./AQWidget.h /gg/include/g++-3/vector.h
AQListView.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQListView.o: /gg/include/g++-3/stl_config.h
AQListView.o: /gg/m68k-amigaos/include/_G_config.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQListView.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQListView.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQListView.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQListView.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQListView.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQListView.o: /gg/include/g++-3/stl_iterator.h
AQListView.o: /gg/include/g++-3/type_traits.h
AQListView.o: /gg/include/g++-3/stl_construct.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQListView.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQListView.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQListView.o: /gg/os-include/machine/ansi.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQListView.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQListView.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQListView.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQListView.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQListView.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQListView.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQListView.o: /gg/os-include/intuition/intuition.h
AQListView.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQListView.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQListView.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQListView.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQListView.o: /gg/os-include/graphics/copper.h
AQListView.o: /gg/os-include/graphics/gfxnodes.h
AQListView.o: /gg/os-include/graphics/monitor.h
AQListView.o: /gg/os-include/graphics/displayinfo.h
AQListView.o: /gg/os-include/graphics/modeid.h
AQListView.o: /gg/os-include/utility/tagitem.h
AQListView.o: /gg/os-include/hardware/custom.h
AQListView.o: /gg/os-include/graphics/rastport.h
AQListView.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQListView.o: /gg/os-include/devices/inputevent.h
AQListView.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQListView.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQListView.o: /gg/os-include/intuition/iobsolete.h
AQListView.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQListView.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h
AQListView.o: /gg/include/g++-3/algorithm /gg/include/g++-3/stl_tempbuf.h
AQListView.o: /gg/include/g++-3/stl_algo.h /gg/include/g++-3/stl_heap.h
AQListView.o: /gg/os-include/graphics/gfxbase.h
AQListView.o: /gg/os-include/exec/libraries.h
AQListView.o: /gg/os-include/exec/interrupts.h
AQListView.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQListView.o: /gg/os-include/clib/graphics_protos.h
AQListView.o: /gg/os-include/graphics/gels.h
AQListView.o: /gg/os-include/graphics/regions.h
AQListView.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQListView.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/layers.h
AQListView.o: /gg/os-include/clib/layers_protos.h
AQListView.o: /gg/os-include/inline/layers.h ./AQScrollBar.h
AQMenu.o: AQMenu.h ./AQWidget.h /gg/include/g++-3/vector.h
AQMenu.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQMenu.o: /gg/include/g++-3/stl_config.h /gg/m68k-amigaos/include/_G_config.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQMenu.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQMenu.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQMenu.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQMenu.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQMenu.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQMenu.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
AQMenu.o: /gg/include/g++-3/stl_construct.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQMenu.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQMenu.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQMenu.o: /gg/os-include/machine/ansi.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQMenu.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQMenu.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQMenu.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQMenu.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQMenu.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQMenu.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQMenu.o: /gg/os-include/intuition/intuition.h /gg/os-include/graphics/clip.h
AQMenu.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQMenu.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQMenu.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQMenu.o: /gg/os-include/graphics/view.h /gg/os-include/graphics/copper.h
AQMenu.o: /gg/os-include/graphics/gfxnodes.h
AQMenu.o: /gg/os-include/graphics/monitor.h
AQMenu.o: /gg/os-include/graphics/displayinfo.h
AQMenu.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
AQMenu.o: /gg/os-include/hardware/custom.h /gg/os-include/graphics/rastport.h
AQMenu.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQMenu.o: /gg/os-include/devices/inputevent.h /gg/os-include/devices/timer.h
AQMenu.o: /gg/os-include/exec/types.h /gg/os-include/exec/io.h
AQMenu.o: /gg/os-include/intuition/screens.h
AQMenu.o: /gg/os-include/intuition/iobsolete.h
AQMenu.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQMenu.o: AQString.h /gg/os-include/dos/dos.h
AQMenu.o: /gg/os-include/graphics/gfxmacros.h
AQMenu.o: /gg/os-include/graphics/gfxbase.h /gg/os-include/exec/libraries.h
AQMenu.o: /gg/os-include/exec/interrupts.h /gg/os-include/proto/graphics.h
AQMenu.o: /gg/os-include/graphics/scale.h
AQMenu.o: /gg/os-include/clib/graphics_protos.h
AQMenu.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQMenu.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQMenu.o: /gg/os-include/inline/graphics.h AQApplication.h
AQScrollBar.o: ./AQScrollBar.h ./AQWidget.h /gg/include/g++-3/vector.h
AQScrollBar.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQScrollBar.o: /gg/include/g++-3/stl_config.h
AQScrollBar.o: /gg/m68k-amigaos/include/_G_config.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQScrollBar.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQScrollBar.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQScrollBar.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQScrollBar.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQScrollBar.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQScrollBar.o: /gg/include/g++-3/stl_iterator.h
AQScrollBar.o: /gg/include/g++-3/type_traits.h
AQScrollBar.o: /gg/include/g++-3/stl_construct.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQScrollBar.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQScrollBar.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQScrollBar.o: /gg/os-include/machine/ansi.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQScrollBar.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQScrollBar.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQScrollBar.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQScrollBar.o: /gg/include/g++-3/stl_uninitialized.h
AQScrollBar.o: /gg/include/g++-3/alloc.h /gg/include/g++-3/stl_alloc.h
AQScrollBar.o: /gg/m68k-amigaos/include/assert.h
AQScrollBar.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQScrollBar.o: /gg/os-include/intuition/intuition.h
AQScrollBar.o: /gg/os-include/graphics/clip.h
AQScrollBar.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQScrollBar.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQScrollBar.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQScrollBar.o: /gg/os-include/graphics/view.h
AQScrollBar.o: /gg/os-include/graphics/copper.h
AQScrollBar.o: /gg/os-include/graphics/gfxnodes.h
AQScrollBar.o: /gg/os-include/graphics/monitor.h
AQScrollBar.o: /gg/os-include/graphics/displayinfo.h
AQScrollBar.o: /gg/os-include/graphics/modeid.h
AQScrollBar.o: /gg/os-include/utility/tagitem.h
AQScrollBar.o: /gg/os-include/hardware/custom.h
AQScrollBar.o: /gg/os-include/graphics/rastport.h
AQScrollBar.o: /gg/os-include/graphics/layers.h
AQScrollBar.o: /gg/os-include/graphics/text.h
AQScrollBar.o: /gg/os-include/devices/inputevent.h
AQScrollBar.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQScrollBar.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQScrollBar.o: /gg/os-include/intuition/iobsolete.h
AQScrollBar.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQScrollBar.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h
AQScrollBar.o: /gg/os-include/graphics/gfxbase.h
AQScrollBar.o: /gg/os-include/exec/libraries.h
AQScrollBar.o: /gg/os-include/exec/interrupts.h
AQScrollBar.o: /gg/os-include/proto/graphics.h
AQScrollBar.o: /gg/os-include/graphics/scale.h
AQScrollBar.o: /gg/os-include/clib/graphics_protos.h
AQScrollBar.o: /gg/os-include/graphics/gels.h
AQScrollBar.o: /gg/os-include/graphics/regions.h
AQScrollBar.o: /gg/os-include/graphics/sprite.h
AQScrollBar.o: /gg/os-include/hardware/blit.h
AQScrollBar.o: /gg/os-include/inline/graphics.h
AQSplitter.o: /gg/os-include/graphics/gfxbase.h
AQSplitter.o: /gg/os-include/exec/libraries.h
AQSplitter.o: /gg/os-include/exec/interrupts.h
AQSplitter.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQSplitter.o: /gg/os-include/clib/graphics_protos.h
AQSplitter.o: /gg/os-include/graphics/gels.h
AQSplitter.o: /gg/os-include/graphics/regions.h
AQSplitter.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQSplitter.o: /gg/os-include/inline/graphics.h AQLayout.h
AQSplitter.o: /gg/os-include/graphics/gfx.h AQKernel.h ./AQWidget.h
AQSplitter.o: /gg/include/g++-3/vector.h /gg/include/g++-3/algobase.h
AQSplitter.o: /gg/include/g++-3/pair.h /gg/include/g++-3/stl_config.h
AQSplitter.o: /gg/m68k-amigaos/include/_G_config.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQSplitter.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQSplitter.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQSplitter.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQSplitter.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQSplitter.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQSplitter.o: /gg/include/g++-3/stl_iterator.h
AQSplitter.o: /gg/include/g++-3/type_traits.h
AQSplitter.o: /gg/include/g++-3/stl_construct.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQSplitter.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQSplitter.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQSplitter.o: /gg/os-include/machine/ansi.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQSplitter.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQSplitter.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQSplitter.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQSplitter.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQSplitter.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQSplitter.o: /gg/include/g++-3/stl_vector.h
AQSplitter.o: /gg/os-include/intuition/intuition.h
AQSplitter.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQSplitter.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQSplitter.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQSplitter.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQSplitter.o: /gg/os-include/graphics/copper.h
AQSplitter.o: /gg/os-include/graphics/gfxnodes.h
AQSplitter.o: /gg/os-include/graphics/monitor.h
AQSplitter.o: /gg/os-include/graphics/displayinfo.h
AQSplitter.o: /gg/os-include/graphics/modeid.h
AQSplitter.o: /gg/os-include/utility/tagitem.h
AQSplitter.o: /gg/os-include/hardware/custom.h
AQSplitter.o: /gg/os-include/graphics/rastport.h
AQSplitter.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQSplitter.o: /gg/os-include/devices/inputevent.h
AQSplitter.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQSplitter.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQSplitter.o: /gg/os-include/intuition/iobsolete.h
AQSplitter.o: /gg/os-include/intuition/preferences.h ./AQObject.h AQString.h
AQSplitter.o: /gg/os-include/dos/dos.h ./AQSplitter.h
AQTextEdit.o: ./AQTextEdit.h ./AQWidget.h /gg/include/g++-3/vector.h
AQTextEdit.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQTextEdit.o: /gg/include/g++-3/stl_config.h
AQTextEdit.o: /gg/m68k-amigaos/include/_G_config.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQTextEdit.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQTextEdit.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQTextEdit.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQTextEdit.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQTextEdit.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQTextEdit.o: /gg/include/g++-3/stl_iterator.h
AQTextEdit.o: /gg/include/g++-3/type_traits.h
AQTextEdit.o: /gg/include/g++-3/stl_construct.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQTextEdit.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQTextEdit.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQTextEdit.o: /gg/os-include/machine/ansi.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQTextEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQTextEdit.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQTextEdit.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQTextEdit.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQTextEdit.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQTextEdit.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQTextEdit.o: /gg/os-include/intuition/intuition.h
AQTextEdit.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQTextEdit.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQTextEdit.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQTextEdit.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQTextEdit.o: /gg/os-include/graphics/copper.h
AQTextEdit.o: /gg/os-include/graphics/gfxnodes.h
AQTextEdit.o: /gg/os-include/graphics/monitor.h
AQTextEdit.o: /gg/os-include/graphics/displayinfo.h
AQTextEdit.o: /gg/os-include/graphics/modeid.h
AQTextEdit.o: /gg/os-include/utility/tagitem.h
AQTextEdit.o: /gg/os-include/hardware/custom.h
AQTextEdit.o: /gg/os-include/graphics/rastport.h
AQTextEdit.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQTextEdit.o: /gg/os-include/devices/inputevent.h
AQTextEdit.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQTextEdit.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQTextEdit.o: /gg/os-include/intuition/iobsolete.h
AQTextEdit.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
AQTextEdit.o: AQString.h /gg/os-include/dos/dos.h
AQTextEdit.o: /gg/os-include/graphics/gfxbase.h
AQTextEdit.o: /gg/os-include/exec/libraries.h
AQTextEdit.o: /gg/os-include/exec/interrupts.h
AQTextEdit.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQTextEdit.o: /gg/os-include/clib/graphics_protos.h
AQTextEdit.o: /gg/os-include/graphics/gels.h
AQTextEdit.o: /gg/os-include/graphics/regions.h
AQTextEdit.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQTextEdit.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/layers.h
AQTextEdit.o: /gg/os-include/clib/layers_protos.h
AQTextEdit.o: /gg/os-include/inline/layers.h ./AQTextDoc.h ./AQScrollBar.h
AQTextEdit.o: ./AQIcon.h AQMenu.h AQApplication.h ./AQClipboard.h
AQTextEdit.o: ./AQAction.h /usr/include/stdio.h
AQTextDoc.o: /gg/include/g++-3/algorithm /gg/include/g++-3/stl_algobase.h
AQTextDoc.o: /gg/include/g++-3/type_traits.h /usr/include/string.h
AQTextDoc.o: /gg/os-include/machine/ansi.h /usr/include/sys/cdefs.h
AQTextDoc.o: /gg/os-include/machine/cdefs.h
AQTextDoc.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQTextDoc.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQTextDoc.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQTextDoc.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQTextDoc.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQTextDoc.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQTextDoc.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQTextDoc.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQTextDoc.o: /gg/include/g++-3/iostream.h /gg/include/g++-3/streambuf.h
AQTextDoc.o: /gg/include/g++-3/libio.h /gg/m68k-amigaos/include/_G_config.h
AQTextDoc.o: /gg/include/g++-3/stl_construct.h
AQTextDoc.o: /gg/include/g++-3/stl_uninitialized.h
AQTextDoc.o: /gg/include/g++-3/stl_tempbuf.h /gg/include/g++-3/stl_algo.h
AQTextDoc.o: /gg/include/g++-3/stl_heap.h ./AQTextDoc.h
AQTextDoc.o: /gg/include/g++-3/vector.h /gg/include/g++-3/algobase.h
AQTextDoc.o: /gg/include/g++-3/pair.h /gg/include/g++-3/stl_config.h
AQTextDoc.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQTextDoc.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQTextDoc.o: /gg/include/g++-3/stl_function.h
AQTextDoc.o: /gg/include/g++-3/stl_iterator.h
AQTextDoc.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQTextDoc.o: /gg/include/g++-3/alloc.h /gg/include/g++-3/stl_alloc.h
AQTextDoc.o: /gg/m68k-amigaos/include/assert.h /gg/include/g++-3/stl_vector.h
AQTextDoc.o: AQKernel.h ./AQObject.h AQString.h /gg/os-include/dos/dos.h
AQTextDoc.o: /gg/os-include/graphics/gfxbase.h
AQTextDoc.o: /gg/os-include/exec/libraries.h /gg/os-include/exec/interrupts.h
AQTextDoc.o: /gg/os-include/graphics/rastport.h
AQTextDoc.o: /gg/os-include/proto/graphics.h /gg/os-include/graphics/scale.h
AQTextDoc.o: /gg/os-include/clib/graphics_protos.h
AQTextDoc.o: /gg/os-include/graphics/gels.h /gg/os-include/graphics/regions.h
AQTextDoc.o: /gg/os-include/graphics/sprite.h /gg/os-include/hardware/blit.h
AQTextDoc.o: /gg/os-include/inline/graphics.h /gg/os-include/proto/dos.h
AQTextDoc.o: /gg/os-include/clib/dos_protos.h /gg/os-include/dos/dosextens.h
AQTextDoc.o: /gg/os-include/dos/record.h /gg/os-include/dos/rdargs.h
AQTextDoc.o: /gg/os-include/dos/dosasl.h /gg/os-include/dos/var.h
AQTextDoc.o: /gg/os-include/dos/notify.h /gg/os-include/dos/datetime.h
AQTextDoc.o: /gg/os-include/inline/dos.h
AQLineEdit.o: ./AQTextDoc.h /gg/include/g++-3/vector.h
AQLineEdit.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQLineEdit.o: /gg/include/g++-3/stl_config.h
AQLineEdit.o: /gg/m68k-amigaos/include/_G_config.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQLineEdit.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQLineEdit.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQLineEdit.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQLineEdit.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQLineEdit.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQLineEdit.o: /gg/include/g++-3/stl_iterator.h
AQLineEdit.o: /gg/include/g++-3/type_traits.h
AQLineEdit.o: /gg/include/g++-3/stl_construct.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQLineEdit.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQLineEdit.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQLineEdit.o: /gg/os-include/machine/ansi.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQLineEdit.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQLineEdit.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQLineEdit.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
AQLineEdit.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
AQLineEdit.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
AQLineEdit.o: /gg/include/g++-3/stl_vector.h AQKernel.h ./AQObject.h
AQLineEdit.o: AQString.h /gg/os-include/dos/dos.h ./AQLineEdit.h
AQLineEdit.o: ./AQTextEdit.h ./AQWidget.h /gg/os-include/graphics/gfx.h
AQLineEdit.o: /gg/os-include/intuition/intuition.h
AQLineEdit.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
AQLineEdit.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
AQLineEdit.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
AQLineEdit.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
AQLineEdit.o: /gg/os-include/graphics/copper.h
AQLineEdit.o: /gg/os-include/graphics/gfxnodes.h
AQLineEdit.o: /gg/os-include/graphics/monitor.h
AQLineEdit.o: /gg/os-include/graphics/displayinfo.h
AQLineEdit.o: /gg/os-include/graphics/modeid.h
AQLineEdit.o: /gg/os-include/utility/tagitem.h
AQLineEdit.o: /gg/os-include/hardware/custom.h
AQLineEdit.o: /gg/os-include/graphics/rastport.h
AQLineEdit.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
AQLineEdit.o: /gg/os-include/devices/inputevent.h
AQLineEdit.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQLineEdit.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQLineEdit.o: /gg/os-include/intuition/iobsolete.h
AQLineEdit.o: /gg/os-include/intuition/preferences.h /usr/include/stdio.h
AQMainWindow.o: AQMainWindow.h ./AQWidget.h /gg/include/g++-3/vector.h
AQMainWindow.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
AQMainWindow.o: /gg/include/g++-3/stl_config.h
AQMainWindow.o: /gg/m68k-amigaos/include/_G_config.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
AQMainWindow.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
AQMainWindow.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
AQMainWindow.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
AQMainWindow.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
AQMainWindow.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
AQMainWindow.o: /gg/include/g++-3/stl_iterator.h
AQMainWindow.o: /gg/include/g++-3/type_traits.h
AQMainWindow.o: /gg/include/g++-3/stl_construct.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
AQMainWindow.o: /gg/include/g++-3/stl_raw_storage_iter.h
AQMainWindow.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
AQMainWindow.o: /gg/os-include/machine/ansi.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
AQMainWindow.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
AQMainWindow.o: /usr/include/stdlib.h /usr/include/sys/types.h
AQMainWindow.o: /gg/os-include/machine/types.h
AQMainWindow.o: /gg/os-include/machine/endian.h
AQMainWindow.o: /gg/include/g++-3/stl_uninitialized.h
AQMainWindow.o: /gg/include/g++-3/alloc.h /gg/include/g++-3/stl_alloc.h
AQMainWindow.o: /gg/m68k-amigaos/include/assert.h
AQMainWindow.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
AQMainWindow.o: /gg/os-include/intuition/intuition.h
AQMainWindow.o: /gg/os-include/graphics/clip.h
AQMainWindow.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
AQMainWindow.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
AQMainWindow.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
AQMainWindow.o: /gg/os-include/graphics/view.h
AQMainWindow.o: /gg/os-include/graphics/copper.h
AQMainWindow.o: /gg/os-include/graphics/gfxnodes.h
AQMainWindow.o: /gg/os-include/graphics/monitor.h
AQMainWindow.o: /gg/os-include/graphics/displayinfo.h
AQMainWindow.o: /gg/os-include/graphics/modeid.h
AQMainWindow.o: /gg/os-include/utility/tagitem.h
AQMainWindow.o: /gg/os-include/hardware/custom.h
AQMainWindow.o: /gg/os-include/graphics/rastport.h
AQMainWindow.o: /gg/os-include/graphics/layers.h
AQMainWindow.o: /gg/os-include/graphics/text.h
AQMainWindow.o: /gg/os-include/devices/inputevent.h
AQMainWindow.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
AQMainWindow.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
AQMainWindow.o: /gg/os-include/intuition/iobsolete.h
AQMainWindow.o: /gg/os-include/intuition/preferences.h AQKernel.h
AQMainWindow.o: ./AQObject.h AQString.h /gg/os-include/dos/dos.h
AQMainWindow.o: /gg/os-include/graphics/gfxbase.h
AQMainWindow.o: /gg/os-include/exec/libraries.h
AQMainWindow.o: /gg/os-include/exec/interrupts.h
AQMainWindow.o: /gg/os-include/proto/graphics.h
AQMainWindow.o: /gg/os-include/graphics/scale.h
AQMainWindow.o: /gg/os-include/clib/graphics_protos.h
AQMainWindow.o: /gg/os-include/graphics/gels.h
AQMainWindow.o: /gg/os-include/graphics/regions.h
AQMainWindow.o: /gg/os-include/graphics/sprite.h
AQMainWindow.o: /gg/os-include/hardware/blit.h
AQMainWindow.o: /gg/os-include/inline/graphics.h ./AQSplitter.h AQLayout.h
DevStudio.o: ./AQScrollBar.h ./AQWidget.h /gg/include/g++-3/vector.h
DevStudio.o: /gg/include/g++-3/algobase.h /gg/include/g++-3/pair.h
DevStudio.o: /gg/include/g++-3/stl_config.h
DevStudio.o: /gg/m68k-amigaos/include/_G_config.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
DevStudio.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
DevStudio.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
DevStudio.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
DevStudio.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
DevStudio.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
DevStudio.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
DevStudio.o: /gg/include/g++-3/stl_construct.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
DevStudio.o: /gg/include/g++-3/stl_raw_storage_iter.h
DevStudio.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
DevStudio.o: /gg/os-include/machine/ansi.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
DevStudio.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
DevStudio.o: /usr/include/stdlib.h /usr/include/sys/types.h
DevStudio.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
DevStudio.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
DevStudio.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
DevStudio.o: /gg/include/g++-3/stl_vector.h /gg/os-include/graphics/gfx.h
DevStudio.o: /gg/os-include/intuition/intuition.h
DevStudio.o: /gg/os-include/graphics/clip.h /gg/os-include/exec/semaphores.h
DevStudio.o: /gg/os-include/exec/nodes.h /gg/os-include/exec/lists.h
DevStudio.o: /gg/os-include/exec/ports.h /gg/os-include/exec/tasks.h
DevStudio.o: /gg/os-include/utility/hooks.h /gg/os-include/graphics/view.h
DevStudio.o: /gg/os-include/graphics/copper.h
DevStudio.o: /gg/os-include/graphics/gfxnodes.h
DevStudio.o: /gg/os-include/graphics/monitor.h
DevStudio.o: /gg/os-include/graphics/displayinfo.h
DevStudio.o: /gg/os-include/graphics/modeid.h
DevStudio.o: /gg/os-include/utility/tagitem.h
DevStudio.o: /gg/os-include/hardware/custom.h
DevStudio.o: /gg/os-include/graphics/rastport.h
DevStudio.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
DevStudio.o: /gg/os-include/devices/inputevent.h
DevStudio.o: /gg/os-include/devices/timer.h /gg/os-include/exec/types.h
DevStudio.o: /gg/os-include/exec/io.h /gg/os-include/intuition/screens.h
DevStudio.o: /gg/os-include/intuition/iobsolete.h
DevStudio.o: /gg/os-include/intuition/preferences.h AQKernel.h ./AQObject.h
DevStudio.o: AQString.h /gg/os-include/dos/dos.h ./AQIcon.h AQDialog.h
DevStudio.o: ./AQTextEdit.h ./AQTextDoc.h AQLayout.h ./AQListView.h
DevStudio.o: AQApplication.h ./AQAction.h Project.h DevStudio.h
DevStudio.o: /gg/include/g++-3/map.h /gg/include/g++-3/tree.h
DevStudio.o: /gg/include/g++-3/stl_tree.h /gg/include/g++-3/stl_map.h
DevStudio.o: AQMainWindow.h
Project.o: /gg/os-include/proto/dos.h /gg/os-include/clib/dos_protos.h
Project.o: /gg/os-include/dos/dosextens.h /gg/os-include/dos/record.h
Project.o: /gg/os-include/dos/rdargs.h /gg/os-include/dos/dosasl.h
Project.o: /gg/os-include/dos/var.h /gg/os-include/dos/notify.h
Project.o: /gg/os-include/dos/datetime.h /gg/os-include/inline/dos.h
Project.o: Project.h AQString.h /gg/os-include/dos/dos.h AQKernel.h
Project.o: /gg/include/g++-3/vector.h /gg/include/g++-3/algobase.h
Project.o: /gg/include/g++-3/pair.h /gg/include/g++-3/stl_config.h
Project.o: /gg/m68k-amigaos/include/_G_config.h
Project.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
Project.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
Project.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
Project.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
Project.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
Project.o: /usr/include/sys/cdefs.h /gg/os-include/machine/cdefs.h
Project.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
Project.o: /gg/include/g++-3/stl_construct.h
Project.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
Project.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
Project.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
Project.o: /gg/include/g++-3/stl_raw_storage_iter.h
Project.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
Project.o: /gg/os-include/machine/ansi.h
Project.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
Project.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
Project.o: /usr/include/stdlib.h /usr/include/sys/types.h
Project.o: /gg/os-include/machine/types.h /gg/os-include/machine/endian.h
Project.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
Project.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
Project.o: /gg/include/g++-3/stl_vector.h
main.o: /usr/include/stdio.h /usr/include/sys/types.h
main.o: /gg/os-include/machine/types.h /usr/include/sys/cdefs.h
main.o: /gg/os-include/machine/cdefs.h /gg/os-include/machine/ansi.h
main.o: /gg/os-include/machine/endian.h /usr/include/stdlib.h AQApplication.h
main.o: /gg/include/g++-3/vector.h /gg/include/g++-3/algobase.h
main.o: /gg/include/g++-3/pair.h /gg/include/g++-3/stl_config.h
main.o: /gg/m68k-amigaos/include/_G_config.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/stddef.h
main.o: /gg/include/g++-3/stl_relops.h /gg/include/g++-3/stl_pair.h
main.o: /gg/include/g++-3/iterator.h /gg/include/g++-3/function.h
main.o: /gg/include/g++-3/stl_function.h /gg/include/g++-3/iostream.h
main.o: /gg/include/g++-3/streambuf.h /gg/include/g++-3/libio.h
main.o: /gg/include/g++-3/stl_iterator.h /gg/include/g++-3/type_traits.h
main.o: /gg/include/g++-3/stl_construct.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/new
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/exception
main.o: /gg/include/g++-3/stl_raw_storage_iter.h
main.o: /gg/include/g++-3/stl_algobase.h /usr/include/string.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/limits.h
main.o: /gg/lib/gcc-lib/m68k-amigaos/2.95.3/include/syslimits.h
main.o: /gg/include/g++-3/stl_uninitialized.h /gg/include/g++-3/alloc.h
main.o: /gg/include/g++-3/stl_alloc.h /gg/m68k-amigaos/include/assert.h
main.o: /gg/include/g++-3/stl_vector.h AQKernel.h DevStudio.h
main.o: /gg/include/g++-3/map.h /gg/include/g++-3/tree.h
main.o: /gg/include/g++-3/stl_tree.h /gg/include/g++-3/stl_map.h
main.o: AQMainWindow.h ./AQWidget.h /gg/os-include/graphics/gfx.h
main.o: /gg/os-include/intuition/intuition.h /gg/os-include/graphics/clip.h
main.o: /gg/os-include/exec/semaphores.h /gg/os-include/exec/nodes.h
main.o: /gg/os-include/exec/lists.h /gg/os-include/exec/ports.h
main.o: /gg/os-include/exec/tasks.h /gg/os-include/utility/hooks.h
main.o: /gg/os-include/graphics/view.h /gg/os-include/graphics/copper.h
main.o: /gg/os-include/graphics/gfxnodes.h /gg/os-include/graphics/monitor.h
main.o: /gg/os-include/graphics/displayinfo.h
main.o: /gg/os-include/graphics/modeid.h /gg/os-include/utility/tagitem.h
main.o: /gg/os-include/hardware/custom.h /gg/os-include/graphics/rastport.h
main.o: /gg/os-include/graphics/layers.h /gg/os-include/graphics/text.h
main.o: /gg/os-include/devices/inputevent.h /gg/os-include/devices/timer.h
main.o: /gg/os-include/exec/types.h /gg/os-include/exec/io.h
main.o: /gg/os-include/intuition/screens.h
main.o: /gg/os-include/intuition/iobsolete.h
main.o: /gg/os-include/intuition/preferences.h ./AQObject.h AQString.h
main.o: /gg/os-include/dos/dos.h
