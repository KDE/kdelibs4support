/* This file is part of the KDE libraries
   Copyright (C) 1999 Torben Weis <weis@kde.org>
   Copyright (C) 2007 Matthias Kretz <kretz@kde.org>

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
#ifndef KCOMPONENTDATA_H
#define KCOMPONENTDATA_H

#include <kdelibs4support_export.h>

#ifdef KDELIBS4SUPPORT_NO_DEPRECATED_NOISE
#warning "This file is deprecated."
#endif

#include <ksharedconfig.h>
#include <kaboutdata.h>

class QByteArray;
class QString;
class K4AboutData;
class KComponentDataPrivate;

/**
 * @short Per component data.
 *
 * This class holds a K4AboutData object or only a component name, and a KSharedConfig object.
 * Those objects normally are different per component but the same per
 * instance of one component.
 *
 * The application component data can always be accessed using KComponentData::mainComponent() (or the
 * convenience function KGlobal::dirs() and KSharedConfig::openConfig()) while the
 * component data of the currently active component (mainly used for KParts) can be accessed using
 * KGlobal::activeComponent().
 *
 * @note This class has been deprecated, you should port code away from it;
 * see https://community.kde.org/Frameworks/Porting_Notes#KComponentData for porting notes.
 *
 * @author Torben Weis
 * @author Matthias Kretz <kretz@kde.org>
 */
class KDELIBS4SUPPORT_DEPRECATED_EXPORT_NOISE KComponentData // krazy:exclude=dpointer (implicitly shared)
{
public:
    /**
     * Creates an invalid KComponentData object.
     *
     * @see isValid()
     */
    KComponentData();

    /**
     * Copy constructor.
     *
     * It does not copy the data. The data is shared between the old and new objects.
     */
    KComponentData(const KComponentData &);

    /**
     * Assignment operator.
     *
     * It does not copy the data. The data is shared between the old and new objects.
     *
     * If the data of the left hand side object was only referenced
     * from this object and no referenced KSharedConfig object needs
     * it anymore, it is deleted
     */
    KComponentData &operator=(const KComponentData &);

    /**
     * Returns whether two KComponentData objects reference the same data.
     */
    bool operator==(const KComponentData &) const;

    /**
     * Returns whether two KComponentData objects do not reference the same data.
     */
    bool operator!=(const KComponentData &rhs) const;

    enum MainComponentRegistration {
        RegisterAsMainComponent,
        SkipMainComponentRegistration
    };

    /**
     * Constructor.
     *
     * @param componentName the name of the component.
     * @param catalogName the name of the translation catalog;
     *                    if left empty @p componentName is used
     * @param registerAsMain whether to register the component as the main component
     *                       of the application. This has no effect, if the application
     *                       already has a main component.
     *                       @see KComponentData::mainComponent
     */
    KDELIBS4SUPPORT_DEPRECATED explicit KComponentData(const QByteArray &componentName, const QByteArray &catalogName = QByteArray(),
                            MainComponentRegistration registerAsMain = RegisterAsMainComponent);

    /**
     * Constructor.
     *
     * A copy of the aboutData object is made.
     *
     * @param aboutData data about this component
     * @param registerAsMain whether to register the component as the main component
     *                       of the application. This has no effect, if the application
     *                       already has a main component.
     *                       @see KComponentData::mainComponent
     *
     * @see K4AboutData
     */
    KDELIBS4SUPPORT_DEPRECATED explicit KComponentData(const K4AboutData &aboutData, MainComponentRegistration registerAsMain = RegisterAsMainComponent);
    KDELIBS4SUPPORT_DEPRECATED explicit KComponentData(const K4AboutData *aboutData, MainComponentRegistration registerAsMain = RegisterAsMainComponent);

    /**
     * Destructor.
     */
    virtual ~KComponentData();

    /**
     * Implicit conversion to KAboutData, to be able to call setComponentData(myComponentData)
     * even if the method is now setComponentData(const KAboutData &)
     */
    operator KAboutData() const;

    /**
     * Returns whether this is a valid object.
     *
     * Don't call any functions on invalid objects, that will crash. Assignment (and of course
     * destruction) is the only valid operation you may do.
     */
    bool isValid() const;

    /**
     * Returns the general config object ("appnamerc").
     * @return the KConfig object for the component.
     */
    const KSharedConfig::Ptr &config() const; //krazy:exclude=constref

    /**
     * Returns the about data of this component.
     *
     * @return The about data of the component. If none has been set in the
     *         constructor but a component name was set, a default constructed
     *         K4AboutData object is returned.
     */
    KDELIBS4SUPPORT_DEPRECATED const K4AboutData *aboutData() const;

    /**
     * Sets the about data of this component.
     *
     * @since 4.5
     */
    KDELIBS4SUPPORT_DEPRECATED void setAboutData(const K4AboutData &aboutData);

    /**
     * Returns the name of the component.
     *
     * @return The component name.
     */
    QString componentName() const;

    /**
     * Returns the name of the translation catalog.
     *
     * @return The catalog name.
     */
    QString catalogName() const;

    /**
     * @internal
     * Returns whether a main KComponentData is available.
     * @since 5.0
     */
    static bool hasMainComponent();

    /**
     * Returns the global component data, if one was set.
     * @since 5.0
     */
    static const KComponentData &mainComponent(); //krazy:exclude=constref (don't mess up ref-counting)

    /**
     * The component currently active (useful in a multi-component
     * application, such as a KParts application).
     * Don't use this - it was mainly for KAboutDialog and KBugReport.
     *
     * They now use KAboutData::applicationData() by default, or a specific KAboutData can be given
     * to them. KHelpMenu always creates them with the application data.
     * So this is now obsolete, the about-app and bug-report dialog simply use the app data
     * rather than the active plugin data.
     *
     * @internal
     * @since 5.0 (moved from KGlobal, but later on KComponentData was deprecated anyway)
     */
    static const KComponentData &activeComponent(); //krazy:exclude=constref (don't mess up ref-counting)

    /**
     * Set the active component for use by KAboutDialog and KBugReport.
     * To be used only by a multi-component (KParts) application.
     *
     * Since 5.0, KAboutDialog and KBugReport don't look at this anymore,
     * so consider just removing the call. See activeComponent() for more details.
     *
     * @see activeComponent()
     * @since 5.0 (moved from KGlobal, but later on KComponentData was deprecated anyway)
     */
    static void setActiveComponent(const KComponentData &d);

protected:
    friend class KApplicationPrivate;

    /**
     * Set name of default config file.
     * @param name the name of the default config file
     */
    void setConfigName(const QString &name);

    /** Standard trick to add virtuals later. @internal */
    virtual void virtual_hook(int id, void *data);

private:
    // Ref-counted data
    KComponentDataPrivate *d;
    friend class KComponentDataPrivate;
};

#endif // KCOMPONENTDATA_H
