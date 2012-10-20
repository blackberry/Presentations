// Default empty project template
#ifndef ContactPickerTest_HPP_
#define ContactPickerTest_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ContactPickerTest : public QObject
{
    Q_OBJECT
public:
    ContactPickerTest(bb::cascades::Application *app);
    virtual ~ContactPickerTest() {}
};


#endif /* ContactPickerTest_HPP_ */
