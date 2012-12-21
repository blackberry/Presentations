// Default empty project template
#ifndef CascadesFlashSample_HPP_
#define CascadesFlashSample_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class CascadesFlashSample : public QObject
{
    Q_OBJECT
public:
    CascadesFlashSample(bb::cascades::Application *app);
    virtual ~CascadesFlashSample() {}
};


#endif /* CascadesFlashSample_HPP_ */
