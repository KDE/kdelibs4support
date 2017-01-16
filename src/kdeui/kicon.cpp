/* This file is part of the KDE libraries
    Copyright (C) 2006 Hamish Rodda <rodda@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "kicon.h"

#include "kiconloader.h"
#include "kiconengine.h"

KIcon::KIcon(const QString &iconName, KIconLoader *iconLoader, const QStringList &overlays)
    : QIcon(new KIconEngine(iconName, iconLoader ? iconLoader : KIconLoader::global(), overlays)),
      d(nullptr)
{
}

KIcon::KIcon(const QString &iconName, KIconLoader *iconLoader)
    : QIcon(new KIconEngine(iconName, iconLoader ? iconLoader : KIconLoader::global())),
      d(nullptr)
{
}

KIcon::KIcon(const QString &iconName)
    : QIcon(new KIconEngine(iconName, KIconLoader::global())),
      d(nullptr)
{
}

KIcon::KIcon()
    : d(nullptr)
{
}

KIcon::KIcon(const QIcon &copy)
    : QIcon(copy),
      d(nullptr)
{
}

KIcon::~KIcon()
{
    //delete d;
}
