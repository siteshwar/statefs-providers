#!/usr/bin/python

import os, sys, re, itertools

template_re = re.compile(r'.*@@([a-z0-9A-Z_-]+)@@.*')

class Actions:

    qt5_system = ["bluez", "upower", "connman", "ofono", "mce"
                  , "keyboard_generic"]
    qt5_user = ["profile"]

    old_names = { "keyboard_generic" : "keyboard-generic" }

    inout_system = ["bluetooth", "power", "network", "cellular", "mce"
                    , "keyboard", "location"]
    inout_user = ["profile"]

    def __init__(self, l):
        self.line = l

    def replace__(self, tpl, src, **kwargs):
        res = ""
        for name in src:
            old_name = Actions.old_names.get(name, name)
            res += tpl.format(name = name, old_name=old_name, **kwargs)
        return res.split("\n")

    def providers(self, name):
        src = getattr(Actions, name)
        with open(name + "-providers.spec.tpl") as f:
            return self.replace__(''.join(f.readlines()), src)

    def install__(self, name, kind):
        src = getattr(Actions, '_'.join((name, kind)))
        with open(name + "-install.spec.tpl") as f:
            return self.replace__(''.join(f.readlines()), src, kind = kind)

    def install(self, name):
        res = self.install__(name, "system")
        res.extend(self.install__(name, "user"))
        return res

def replaced(l):
    m = template_re.match(l)
    if (m is None):
        return (l,)
    actions = Actions(l)
    (part, name) = m.group(1).split("-")
    return getattr(actions, part)(name)

Actions.inout_user = ["inout_" + a for a in Actions.inout_user]
Actions.inout_system = ["inout_" + a for a in Actions.inout_system]

with open("statefs-providers.spec.tpl") as f:
    lines = f.readlines()
    res = itertools.chain(*[replaced(l.strip()) for l in lines])
    #res = list(itertools.chain(*[replaced(l) for l in res]))
    with open("statefs-providers.spec", "w") as out:
        out.write('\n'.join(list(res)))
        out.write('\n')
