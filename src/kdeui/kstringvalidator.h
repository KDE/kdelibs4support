/*
    kstringvalidator.h

    Copyright (c) 2001 Marc Mutz <mutz@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; version 2.0
    of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301 USA
*/

#ifndef KSTRINGVALIDATOR_H
#define KSTRINGVALIDATOR_H

#include <kdelibs4support_export.h>

#include <QStringList>
#include <QValidator>

/**
 * @deprecated since 5.0
 * @short A QValidator to (dis)allow certain strings
 *
 * This validator allows you to accept only certain or to accept all
 * but certain strings.
 *
 * When used in rejecting mode, accepts only strings not in the
 * stringlist. This mode is the default and comes in handy when asking
 * the user for a name of some listed entity. Set the list of already
 * used names to prevent the user from entering duplicate names.
 *
 * When used in non-rejecting mode, accepts only strings that appear
 * in the stringlist. Use with care! From a user's point of view this
 * mode is hard to grasp.
 *
 * This validator can also fix strings. In rejecting mode, a number
 * will be appended to the string until it is Acceptable. E.g. if
 * "foo" and "foo 1" are in the stringlist, then fixup will change
 * "foo" to "foo 2", provided "foo 2" isn't in the list of forbidden
 * strings.
 *
 * In accepting mode, when the input starts with an Acceptable
 * substring, truncates to the longest Acceptable string. When the
 * input is the start of an Acceptable string, completes to the
 * shortest Acceptable string.
 *
 * NOTE: fixup isn't yet implemented.
 *
 * @author Marc Mutz <mutz@kde.org>
 **/
class KDELIBS4SUPPORT_DEPRECATED_EXPORT KStringListValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(QStringList stringList READ stringList WRITE setStringList)
    Q_PROPERTY(bool rejecting READ isRejecting WRITE setRejecting)
    Q_PROPERTY(bool fixupEnabled READ isFixupEnabled WRITE setFixupEnabled)

public:
    /**
     * Creates a new string validator.
     *
     * @param list         The list of strings to (dis)allow.
     * @param rejecting    Selects the validator's mode
     *                     (rejecting: true; accepting: false)
     * @param fixupEnabled Selects whether to fix strings or not.
     * @param parent Passed to lower level constructor.
     *
     **/
    KDELIBS4SUPPORT_DEPRECATED explicit KStringListValidator(const QStringList &list = QStringList(),
                                  bool rejecting = true, bool fixupEnabled = false,
                                  QObject *parent = nullptr);

    /**
     * Destroys the string validator.
     */
    ~KStringListValidator() override;

    /**
     * Sets whether the string validator is in rejecting mode or not.
     * If in rejecting mode, the strings from @see stringList are not
     * allowed to appear in the validation string.
     */
    void setRejecting(bool rejecting);

    /**
     * Returns whether the string validator is in rejecting mode.
     */
    bool isRejecting() const;

    /**
     * Sets the fixup flag. If enabled, wrong input is corrected
     * automatically.
     */
    void setFixupEnabled(bool fixupEnabled);

    /**
     * Returns whether the fixup flag is set.
     */
    bool isFixupEnabled() const;

    /**
     * Sets the @param list of string which is used as black or
     * white list, depending on the rejecting mode (@see isRejecting()).
     */
    void setStringList(const QStringList &list);

    /**
     * Returns the string list of the validator.
     */
    QStringList stringList() const;

    /**
     * Reimplemented from @see QValidator.
     */
    State validate(QString &input, int &pos) const override;

    /**
     * Reimplemented from @see QValidator.
     */
    void fixup(QString &input) const override;

private:
    class Private;
    Private *const d;
};

#endif // KSTRINGVALIDATOR_H
