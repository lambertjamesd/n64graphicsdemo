#!smake
# --------------------------------------------------------------------
#        Copyright (C) 1998 Nintendo. (Originated by SGI)
#        
#        $RCSfile: Makefile,v $
#        $Revision: 1.1.1.1 $
#        $Date: 2002/05/02 03:27:21 $
# --------------------------------------------------------------------
include /usr/include/n64/make/PRdefs

MIDICVT:=tools/midicvt
SFZ2N64:=tools/sfz2n64
SKELETOOL64?=skeletool64

OPTIMIZER		:= -O0
LCDEFS			:= -DDEBUG -g -Isrc/ -I/usr/include/n64/nustd -Werror -Wall
N64LIB			:= -lultra_rom -lnustd

ifeq ($(WITH_DEBUGGER),1)
LCDEFS += -DWITH_DEBUGGER
endif

BASE_TARGET_NAME = build/normalmapping

LD_SCRIPT	= normalmapping.ld
CP_LD_SCRIPT	= build/normalmapping

ASMFILES    =	$(shell find asm/ -type f -name '*.s')

ASMOBJECTS  =	$(patsubst %.s, build/%.o, $(ASMFILES))

CODEFILES = $(shell find src/ -type f -name '*.c')

CODEOBJECTS = $(patsubst %.c, build/%.o, $(CODEFILES))

CODESEGMENT =	build/codesegment

BOOT		=	/usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	=	build/boot.6102.o

OBJECTS		=	$(ASMOBJECTS) $(BOOT_OBJ)

DEPS = $(patsubst %.c, build/%.d, $(CODEFILES)) $(patsubst %.c, build/%.d, $(DATAFILES))

-include $(DEPS)

LCINCS =	-I/usr/include/n64/PR 
LCDEFS +=	-DF3DEX_GBI_2
#LCDEFS +=	-DF3DEX_GBI_2 -DFOG
#LCDEFS +=	-DF3DEX_GBI_2 -DFOG -DXBUS
#LCDEFS +=	-DF3DEX_GBI_2 -DFOG -DXBUS -DSTOP_AUDIO

LDIRT  =	$(BASE_TARGET_NAME).elf $(CP_LD_SCRIPT) $(BASE_TARGET_NAME).z64 $(BASE_TARGET_NAME)_no_debug.map $(ASMOBJECTS)

LDFLAGS =	-L/usr/lib/n64 $(N64LIB)  -L$(N64_LIBGCCDIR) -lgcc

default:	$(BASE_TARGET_NAME).z64

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

build/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MM $^ -MF "$(@:.o=.d)" -MT"$@"
	$(CC) $(CFLAGS) -c -o $@ $<

build/%.o: %.s
	@mkdir -p $(@D)
	$(AS) -Wa,-Iasm -o $@ $<

####################
## Assets
####################

build/assets/brick_normal_x.png: assets/brick_normals.png
	@mkdir -p $(@D)
	convert assets/brick_normals.png -channel r -separate build/assets/brick_normal_x.png

build/assets/brick_normal_y.png: assets/brick_normals.png
	@mkdir -p $(@D)
	convert assets/brick_normals.png -channel g -separate build/assets/brick_normal_y.png

build/assets/brick_normal_z.png: assets/brick_normals.png
	@mkdir -p $(@D)
	convert assets/brick_normals.png -channel b -separate build/assets/brick_normal_z.png

DERIVED_IMAGES = build/assets/brick_normal_x.png build/assets/brick_normal_y.png build/assets/brick_normal_z.png

build/assets/materials.h: assets/materials.skm.yaml \
	$(DERIVED_IMAGES) \
	assets/brick_texture.png
	@mkdir -p $(@D)
	$(SKELETOOL64) --name materials -m assets/materials.skm.yaml --material-output -o build/assets/materials.h


MESH_LIST = brick_color brick_normal brick_tangent brick_bitangent brick_ntangent brick_nbitangent

MESH_HEADERS = $(MESH_LIST:%=build/assets/%.h)
MESH_SOURCE = $(MESH_LIST:%=build/assets/%_geo.c)
MESH_OBJECTS = $(MESH_LIST:%=build/assets/%_geo.o)

build/assets/%.h: assets/brick_model.fbx assets/materials.skm.yaml $(DERIVED_IMAGES)
	@mkdir -p $(@D) 
	$(SKELETOOL64) --name $(@:build/assets/%.h=%) \
		--fixed-point-scale 256 --model-scale 0.01 \
		-m assets/materials.skm.yaml --default-material $(@:build/assets/%.h=%) --force-material cube_color \
		-o $@ assets/brick_model.fbx

build/assets/%.o: assets/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MM $^ -MF "$(@:.o=.d)" -MT"$@"
	$(CC) $(CFLAGS) -c -o $@ $<

src/models/models.c: $(MESH_HEADERS)

####################
## Linking
####################

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

# without debugger

CODEOBJECTS_NO_DEBUG = $(CODEOBJECTS) $(MESH_OBJECTS)

ifeq ($(WITH_DEBUGGER),1)
CODEOBJECTS_NO_DEBUG += build/debugger/debugger_stub.o build/debugger/serial.o 
endif

$(CODESEGMENT)_no_debug.o:	$(CODEOBJECTS_NO_DEBUG)
	$(LD) -o $(CODESEGMENT)_no_debug.o -r $(CODEOBJECTS_NO_DEBUG) $(LDFLAGS)


$(CP_LD_SCRIPT)_no_debug.ld: $(LD_SCRIPT)
	cpp -P -Wno-trigraphs $(LCDEFS) -DCODE_SEGMENT=$(CODESEGMENT)_no_debug.o -o $@ $<

$(BASE_TARGET_NAME).z64: $(CODESEGMENT)_no_debug.o $(OBJECTS) $(CP_LD_SCRIPT)_no_debug.ld
	$(LD) -L. -T $(CP_LD_SCRIPT)_no_debug.ld -Map $(BASE_TARGET_NAME)_no_debug.map -o $(BASE_TARGET_NAME).elf
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(BASE_TARGET_NAME).elf $(BASE_TARGET_NAME).z64 -O binary
	makemask $(BASE_TARGET_NAME).z64

# with debugger
CODEOBJECTS_DEBUG = $(CODEOBJECTS) $(MESH_OBJECTS)

ifeq ($(WITH_DEBUGGER),1)
CODEOBJECTS_DEBUG += build/debugger/debugger.o build/debugger/serial.o 
endif

$(CODESEGMENT)_debug.o:	$(CODEOBJECTS_DEBUG)
	$(LD) -o $(CODESEGMENT)_debug.o -r $(CODEOBJECTS_DEBUG) $(LDFLAGS)

$(CP_LD_SCRIPT)_debug.ld: $(LD_SCRIPT)
	cpp -P -Wno-trigraphs $(LCDEFS) -DCODE_SEGMENT=$(CODESEGMENT)_debug.o -o $@ $<

$(BASE_TARGET_NAME)_debug.z64: $(CODESEGMENT)_debug.o $(OBJECTS) $(CP_LD_SCRIPT)_debug.ld
	$(LD) -L. -T $(CP_LD_SCRIPT)_debug.ld -Map $(BASE_TARGET_NAME)_debug.map -o $(BASE_TARGET_NAME)_debug.elf
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(BASE_TARGET_NAME)_debug.elf $(BASE_TARGET_NAME)_debug.z64 -O binary
	makemask $(BASE_TARGET_NAME)_debug.z64

clean:
	rm -rf build