#!/usr/bin/env python
# encoding: utf-8
#
# Convert interface file into set of header and implementation files.
# name_vX.if -> name_vX_interface.h
#            -> name_vX_interface.cpp
#            -> name_vX_impl.h
#
import Task, os
from TaskGen import feature, before, extension
#Task.simple_task_type('if2code',
                      #rule=meddle_file, #'${MEDDLER} ${SRC} ${TGT}',
                      #color='PINK', before='cxx')

class if2code(Task.Task):
    color = 'PINK'
    before = 'cxx'
    def run(self):
        cwd = self.inputs[0].parent.bldpath(self.env)
        # TODO: add include paths
        cmd = '%s %s -o=%s' % (self.env.MEDDLER, self.inputs[0].abspath(), cwd)
        print "Running "+cmd
        import Utils
        return Utils.exec_command(cmd)

@extension('.if')
def compile_idl(self, node):
    c_node = node.parent.find_or_declare(node.file_base() + '_interface.cpp')
    intf_h_node = node.parent.find_or_declare(node.file_base() + '_interface.h')
    impl_h_node = node.parent.find_or_declare(node.file_base() + '_impl.h')
    tsk = self.create_task('if2code', node, [c_node, intf_h_node, impl_h_node])
    self.more_source = [c_node]

    tg = self.bld.name_to_obj('meddler', self.env) # Find location of the compiler binary
    tg.post()

    name = tg.link_task.outputs[0].abspath(tg.env)
    print "Meddler found as "+name
    tsk.env.MEDDLER = name

@feature('*')
@before('apply_core')
def process_add_source(self):
    if not getattr(self, 'add_source', None):
        return

    for x in self.to_list(self.add_source):
        print "Adding "+x+" to list"
        y = self.bld.name_to_obj(x, self.env)
        y.post()

        if getattr(y, 'more_source', None):
            self.allnodes.extend(y.more_source)
